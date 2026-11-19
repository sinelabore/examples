/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Peter Mueller
 */
/*
 * Reference timer service for SinelaboreRT state machines (C99).
 * See timer_service.h. A fixed timer table, searched linearly: fine for the
 * few dozen timers a typical device has. Each running timer holds the ticks
 * it has left, and tsPoll() counts them all down by the time since the last
 * poll; it visits every entry, as the search does anyway.
 */
#include "timer_service.h"

#ifdef TS_ERROR_HANDLER_DEFAULT
#include <stdio.h>
#include <stdlib.h>

/* The default error handler: report and stop. Compiled only without a
 * handler of the application's (TS_ERROR_HANDLER). */
void tsErrorExit(TS_ERROR_T error, const void *machine, TS_TIMER_ID_T timerId)
{
    (void)fprintf(stderr, "timer service: error %d (machine %p, timer %lu)\n",
                  (int)error, machine, (unsigned long)timerId);
    exit(EXIT_FAILURE);
}
#endif

/* True if the call names a service and a machine; else the error is reported. */
static bool usable(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    if (ts == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_SERVICE, machine, timerId);
        return false;
    }
    if (machine == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_MACHINE, machine, timerId);
        return false;
    }
    return true;
}

/* The entry of the timer (machine, timerId) if it runs. NULL
 * is an answer, not an error: the timer does not run. The caller has checked
 * ts and machine (usable()). */
static TS_TIMER_T *entryOf(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    uint16_t i;
    for (i = 0U; i < ts->capacity; i++) {
        if ((ts->table[i].machine == machine) && (ts->table[i].timerId == timerId)) {
            return &ts->table[i];
        }
    }
    return NULL;
}

/* The first free entry of the table, for a timer that starts; NULL when every
 * entry is in use. */
static TS_TIMER_T *firstFreeEntry(TS_SERVICE_T *ts)
{
    uint16_t i;
    for (i = 0U; i < ts->capacity; i++) {
        if (ts->table[i].machine == NULL) {
            return &ts->table[i];
        }
    }
    return NULL;
}

/* The entry of a running timer, for the functions for the time left: NULL if
 * the call is wrong (reported) or the timer does not run (no error). */
static TS_TIMER_T *runningEntry(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    return usable(ts, machine, timerId) ? entryOf(ts, machine, timerId) : NULL;
}

/* Ticks t has left; 0 once it has expired. */
static TS_TICKS_T left(const TS_TIMER_T *t)
{
    return t->pending ? 0U : t->left;
}

/* Give t `ticks` left. An expiry that was not reported yet is taken back. */
static void setLeft(TS_TIMER_T *t, TS_TICKS_T ticks)
{
    t->left = ticks;
    t->pending = false;
    t->periods = 0U;
}

/* Count t down by 'elapsed' ticks; how many times it expired within them, 0
 * for none. A TS_EVERY timer then goes on with its next period: the phase is
 * kept, and each period a late poll missed counts (at most 65535 at a time).
 * Each poll calls it once for every running timer. */
static uint16_t countDown(TS_TIMER_T *t, TS_TICKS_T elapsed)
{
    if (t->left > elapsed) {
        t->left -= elapsed;
        return 0U;
    }
    if (t->every) {
        const TS_TICKS_T late = elapsed - t->left;
        const TS_TICKS_T missed = (TS_TICKS_T)(late / t->period);
        t->left = t->period - (late % t->period);
        return (missed >= (TS_TICKS_T)0xFFFFU) ? 0xFFFFU : (uint16_t)(missed + 1U);
    }
    t->left = 0U;
    return 1U;
}

/* a + b, at most 65535 */
static uint16_t addPeriods(uint16_t a, uint16_t b)
{
    return (b > (uint16_t)(0xFFFFU - a)) ? 0xFFFFU : (uint16_t)(a + b);
}

/* ---- The application ---- */

void tsInit(TS_SERVICE_T *ts, TS_TIMER_T *table, uint16_t capacity, TS_TICKS_T now)
{
    uint16_t i;
    uint16_t entries = capacity;
    if (ts == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_SERVICE, NULL, 0U);
        return;
    }
    if ((table == NULL) && (capacity > 0U)) {
        TS_ERROR_HANDLER(TS_ERR_NO_TABLE, NULL, 0U);
        entries = 0U;                       /* if the handler returns: a service without entries */
    }
    ts->table = table;
    ts->capacity = entries;
    ts->now = now;
    ts->refused = 0U;
    for (i = 0U; i < entries; i++) {
        table[i].machine = NULL;
        table[i].pending = false;
        table[i].periods = 0U;
    }
}

uint16_t tsPoll(TS_SERVICE_T *ts, TS_TICKS_T now, TS_TIMEOUT_T *out, uint16_t max)
{
    uint16_t i;
    uint16_t n = 0U;
    uint16_t room = max;
    TS_TICKS_T elapsed;

    if (ts == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_SERVICE, NULL, 0U);
        return 0U;
    }
    if ((out == NULL) && (max > 0U)) {
        TS_ERROR_HANDLER(TS_ERR_NO_TABLE, NULL, 0U);
        room = 0U;                          /* if the handler returns: count down, report nothing */
    }
    elapsed = now - ts->now;                /* modulo the counter's range: a wrap is harmless */
    ts->now = now;
    for (i = 0U; i < ts->capacity; i++) {
        TS_TIMER_T *t = &ts->table[i];
        if (t->machine != NULL) {
            const uint16_t ended = countDown(t, elapsed);
            if (ended > 0U) {
                t->pending = true;
                t->periods = addPeriods(t->periods, ended);
            }
            if (t->pending && (n < room)) {
                out[n].machine = t->machine;
                out[n].timerId = t->timerId;
                out[n].periods = t->periods;
                n++;
                t->pending = false;
                t->periods = 0U;
                if (!t->every) {
                    t->machine = NULL;
                }
            }
        }
    }
    return n;
}

uint16_t tsRunning(const TS_SERVICE_T *ts)
{
    uint16_t i;
    uint16_t n = 0U;
    if (ts == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_SERVICE, NULL, 0U);
        return 0U;
    }
    for (i = 0U; i < ts->capacity; i++) {
        if (ts->table[i].machine != NULL) {
            n++;
        }
    }
    return n;
}

/* ---- Start and stop: needed by every machine with a timer ---- */

bool tsStart(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId,
             TS_TICKS_T ticks, TS_MODE_T mode)
{
    TS_TIMER_T *t;

    if (!usable(ts, machine, timerId)) {
        return false;
    }
    if ((mode == TS_EVERY) && (ticks == 0U)) {
        TS_ERROR_HANDLER(TS_ERR_NO_PERIOD, machine, timerId);
        return false;
    }
    t = entryOf(ts, machine, timerId);       /* running already: restart it */
    if (t == NULL) {
        t = firstFreeEntry(ts);              /* else a free entry */
    }
    if (t == NULL) {
        ts->refused++;
        TS_ERROR_HANDLER(TS_ERR_TABLE_FULL, machine, timerId);
        return false;
    }
    t->machine = machine;
    t->timerId = timerId;
    t->every = (mode == TS_EVERY);
    t->period = ticks;
    setLeft(t, ticks);
    return true;
}

void tsStop(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    TS_TIMER_T *t = runningEntry(ts, machine, timerId);
    if (t != NULL) {
        t->machine = NULL;
    }
}

void tsStopAll(TS_SERVICE_T *ts, const void *machine)
{
    uint16_t i;
    if (!usable(ts, machine, 0U)) {
        return;
    }
    for (i = 0U; i < ts->capacity; i++) {
        if (ts->table[i].machine == machine) {
            ts->table[i].machine = NULL;
        }
    }
}

/* ---- The time left: needed only for TIME_LEFT and the commands of named timers ---- */

TS_TICKS_T tsTimeLeft(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    uint16_t i;
    if (!usable(ts, machine, timerId)) {
        return 0U;
    }
    for (i = 0U; i < ts->capacity; i++) {
        const TS_TIMER_T *t = &ts->table[i];
        if ((t->machine == machine) && (t->timerId == timerId)) {
            return left(t);
        }
    }
    return 0U;
}

bool tsSetTimeLeft(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T ticks)
{
    TS_TIMER_T *t = runningEntry(ts, machine, timerId);
    if (t == NULL) {
        return false;
    }
    setLeft(t, ticks);
    return true;
}

/* A duration too long for the ticks: reported, and the longest there is. */
static TS_TICKS_T tooLong(void)
{
    TS_ERROR_HANDLER(TS_ERR_TOO_LONG, NULL, 0U);
    return TS_TICKS_MAX;
}

/* tsUs() divides a 64-bit uintmax_t in usWide(). */
typedef char TS_UINTMAX_MUST_HAVE_64_BITS[(sizeof(uintmax_t) == 8U) ? 1 : -1];

/* One step of the long division below: the next 16 bits of the dividend. */
static void usStep(uintmax_t *q, uint32_t *r, uint32_t bits16)
{
    const uint32_t chunk = (*r << 16U) | bits16;   /* *r < TS_TICK_US <= 1000: fits */
    *q = (*q << 16U) | (chunk / TS_TICK_US);
    *r = chunk % TS_TICK_US;
}

/* us / TS_TICK_US, rounded up, for us beyond 32 bits: long division in 16-bit
 * steps, each a 32-bit division, and only shifts by constants, so a 32-bit
 * target needs no 64-bit division or shift routine. us fits (TS_US_MAX). */
static TS_TICKS_T usWide(uintmax_t us)
{
    const uint32_t hi = (uint32_t)(us >> 32U);
    const uint32_t lo = (uint32_t)us;
    uintmax_t q = 0U;
    uint32_t r = 0U;
    usStep(&q, &r, hi >> 16U);
    usStep(&q, &r, hi & 0xFFFFU);
    usStep(&q, &r, lo >> 16U);
    usStep(&q, &r, lo & 0xFFFFU);
    return (TS_TICKS_T)(q + ((r != 0U) ? 1U : 0U));
}

/* Once a duration fits, the conversions compute in TS_TICKS_T, or tsUs() in
 * 32 bits: a 32-bit target needs no 64-bit multiplication or division routine. */
TS_TICKS_T tsUs(uintmax_t us)
{
    if (!TS_US_FITS(us)) {
        return tooLong();
    }
    if (us <= UINT32_MAX) {
        const uint32_t u = (uint32_t)us;
        return (TS_TICKS_T)(u / TS_TICK_US + (((u % TS_TICK_US) != 0U) ? 1U : 0U));
    }
    return usWide(us);
}

TS_TICKS_T tsMs(uintmax_t ms)
{
    if (!TS_MS_FITS(ms)) {
        return tooLong();
    }
    return (TS_TICKS_T)((TS_TICKS_T)ms * (TS_TICKS_T)(1000U / TS_TICK_US));
}

TS_TICKS_T tsS(uintmax_t s)
{
    if (!TS_S_FITS(s)) {
        return tooLong();
    }
    return (TS_TICKS_T)((TS_TICKS_T)s * (TS_TICKS_T)(1000000UL / TS_TICK_US));
}

TS_TICKS_T tsTimeAdd(TS_TICKS_T time, TS_TICKS_T ticks)
{
    return (ticks > TS_TICKS_MAX - time) ? TS_TICKS_MAX : time + ticks;
}

TS_TICKS_T tsTimeSub(TS_TICKS_T time, TS_TICKS_T ticks)
{
    if (ticks >= time) {
        return 0U;
    }
    return time - ticks;
}
