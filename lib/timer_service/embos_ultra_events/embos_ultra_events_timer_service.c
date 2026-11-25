/* SPDX-License-Identifier: MIT */
/* Timer service on embOS-Ultra software timers, timeouts as events; see timer_service.h. */
#include "timer_service.h"

#include <stdio.h>
#include <stdlib.h>

#if TS_TICK_US != 1000U
#error "TS_TICK_US must be 1000: the service counts in the milliseconds of OS_TIMER_SetPeriodEx_ms()"
#endif

#ifdef TS_ERROR_HANDLER_DEFAULT
void tsErrorExit(TS_ERROR_T error, const void *machine, TS_TIMER_ID_T timerId)
{
    (void)fprintf(stderr, "timer service: error %d (machine %p, timer %u)\n",
                  (int)error, machine, (unsigned)timerId);
    exit(EXIT_FAILURE);
}
#endif

/* The timer callback, above every task: report the timeout. embOS timers
 * fire once, so an every timer restarts itself here first, with its period:
 * the time just ended may have been another (tsSetTimeLeft). */
static void onExpiry(void *data)
{
    TS_TIMER_T *t = (TS_TIMER_T *)data;
    if (t->cycle != 0U) {
        OS_TIMER_SetPeriodEx_ms(&t->timer, (OS_U32)t->cycle);
        OS_TIMER_RestartEx(&t->timer);
    }
    t->service->timeout(t->service->context, t->id);
}

void tsInit(TS_SERVICE_T *ts, TS_TIMEOUT_FN timeout, void *context, TS_TIMER_T *timers, uint16_t count)
{
    uint16_t id;
    if (timeout == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_CALLBACK, NULL, 0U);
        return;
    }
    ts->timeout = timeout;
    ts->context = context;
    ts->timers = timers;
    ts->count = count;
    for (id = 0U; id < count; id++) {
        timers[id].service = ts;
        timers[id].id = id;
        timers[id].cycle = 0U;
        /* created stopped; every start sets the period, 1 ms is a placeholder */
        OS_TIMER_CreateEx_ms(&timers[id].timer, onExpiry, 1U, &timers[id]);
    }
}

/* The timer of an id, or NULL after reporting why there is none. */
static TS_TIMER_T *timerOf(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    if (ts == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_SERVICE, machine, timerId);
        return NULL;
    }
    if (machine == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_MACHINE, machine, timerId);
        return NULL;
    }
    if (timerId >= ts->count) {
        TS_ERROR_HANDLER(TS_ERR_NO_TIMER, machine, timerId);
        return NULL;
    }
    return &ts->timers[timerId];
}

/* Let the timer expire `first` ticks from now, then every `cycle` if that is
 * not 0. It is stopped first, so no callback runs until it is restarted: a
 * callback cannot be running meanwhile (it would have preempted this task
 * and finished). A timeout reported before stays with the machine.
 * embOS does not take a period of 0 (OS_ERR_TIMER_PERIOD_INVALID), so a
 * timeout of 0 ticks is reported at once, in the calling task, as the
 * reference service reports it with the next poll. */
static void arm(TS_SERVICE_T *ts, TS_TIMER_T *t, TS_TICKS_T first, TS_TICKS_T cycle)
{
    OS_TIMER_StopEx(&t->timer);
    t->cycle = cycle;
    if (first == 0U) {
        ts->timeout(ts->context, t->id);
        if (cycle == 0U) {
            return;
        }
        first = cycle;
    }
    OS_TIMER_SetPeriodEx_ms(&t->timer, (OS_U32)first);
    OS_TIMER_RestartEx(&t->timer);          /* starts it with that period */
}

bool tsStart(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId,
             TS_TICKS_T ticks, TS_MODE_T mode)
{
    TS_TIMER_T *t = timerOf(ts, machine, timerId);
    if (t == NULL) {
        return false;
    }
    if ((mode == TS_EVERY) && (ticks == 0U)) {
        TS_ERROR_HANDLER(TS_ERR_NO_PERIOD, machine, timerId);
        return false;
    }
    arm(ts, t, ticks, (mode == TS_EVERY) ? ticks : 0U);
    return true;
}

/* No callback follows (see arm()); `cycle` is read only while the timer runs. */
void tsStop(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    TS_TIMER_T *t = timerOf(ts, machine, timerId);
    if (t != NULL) {
        OS_TIMER_StopEx(&t->timer);
    }
}

void tsStopAll(TS_SERVICE_T *ts, const void *machine)
{
    TS_TIMER_ID_T id;
    if (timerOf(ts, machine, 0U) == NULL) {
        return;
    }
    for (id = 0U; id < ts->count; id++) {
        tsStop(ts, machine, id);
    }
}

TS_TICKS_T tsTimeLeft(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    const TS_TIMER_T *t = timerOf(ts, machine, timerId);
    OS_U64 us;
    if ((t == NULL) || (OS_TIMER_GetStatusEx(&t->timer) == 0U)) {
        return 0U;
    }
    us = OS_TIMER_GetRemainingPeriodEx_us(&t->timer);
    return (TS_TICKS_T)((us + (TS_TICK_US - 1U)) / TS_TICK_US);
}

bool tsSetTimeLeft(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T ticks)
{
    TS_TIMER_T *t = timerOf(ts, machine, timerId);
    if ((t == NULL) || (OS_TIMER_GetStatusEx(&t->timer) == 0U)) {
        /* not running; a once timer that expired has posted its timeout,
         * which cannot be taken back */
        return false;
    }
    arm(ts, t, ticks, t->cycle);
    return true;
}

static TS_TICKS_T tooLong(void)
{
    TS_ERROR_HANDLER(TS_ERR_TOO_LONG, NULL, 0U);
    return TS_TICKS_MAX;
}

TS_TICKS_T tsUs(uintmax_t us)
{
    return TS_US_FITS(us) ? TS_US(us) : tooLong();
}

TS_TICKS_T tsMs(uintmax_t ms)
{
    return TS_MS_FITS(ms) ? TS_MS(ms) : tooLong();
}

TS_TICKS_T tsS(uintmax_t s)
{
    return TS_S_FITS(s) ? TS_S(s) : tooLong();
}

TS_TICKS_T tsTimeAdd(TS_TICKS_T time, TS_TICKS_T ticks)
{
    return (ticks > TS_TICKS_MAX - time) ? TS_TICKS_MAX : (TS_TICKS_T)(time + ticks);
}

TS_TICKS_T tsTimeSub(TS_TICKS_T time, TS_TICKS_T ticks)
{
    return (ticks >= time) ? 0U : (TS_TICKS_T)(time - ticks);
}
