/* SPDX-License-Identifier: MIT */
/* Timer service on FreeRTOS software timers; see timer_service.h. */
#include "timer_service.h"

#include <stdio.h>
#include <stdlib.h>

#if (TS_TICK_US * configTICK_RATE_HZ) != 1000000U
#error "TS_TICK_US must be the FreeRTOS tick"
#endif

#ifdef TS_ERROR_HANDLER_DEFAULT
void tsErrorExit(TS_ERROR_T error, const void *machine, TS_TIMER_ID_T timerId)
{
    (void)fprintf(stderr, "timer service: error %d (machine %p, timer %u)\n",
                  (int)error, machine, (unsigned)timerId);
    exit(EXIT_FAILURE);
}
#endif

/* The timer callback, in the timer task: set the timer's bit. */
static void onExpiry(TimerHandle_t handle)
{
    TS_TIMER_T *t = (TS_TIMER_T *)pvTimerGetTimerID(handle);
    if (t->restoreCycle) {
        /* the time left of an every timer was set: the next periods are the old ones */
        t->restoreCycle = false;
        (void)xTimerChangePeriod(handle, (TickType_t)t->cycle, 0U);
    }
    (void)xTaskNotify(t->service->task, TS_BIT(t->id), eSetBits);
}

void tsInit(TS_SERVICE_T *ts, TaskHandle_t task, TS_TIMER_T *timers, uint16_t count)
{
    uint16_t id;
    configASSERT(count <= 24U);         /* bits 24 to 31 stay free for the application */
    ts->task = task;
    ts->timers = timers;
    ts->count = count;
    for (id = 0U; id < count; id++) {
        timers[id].service = ts;
        timers[id].id = id;
        timers[id].cycle = 0U;
        timers[id].restoreCycle = false;
        /* the period is set by every start; 1 is a placeholder (0 is not allowed) */
        timers[id].handle = xTimerCreate("sm", 1U, pdFALSE, &timers[id], onExpiry);
        if (timers[id].handle == NULL) {
            TS_ERROR_HANDLER(TS_ERR_OS, NULL, id);
        }
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

/* Stop the timer and drop a timeout it has set but the task has not taken.
 * The timer task runs the stop at once (highest priority), so no callback
 * can follow. */
static void disarm(const TS_SERVICE_T *ts, const TS_TIMER_T *t)
{
    (void)xTimerStop(t->handle, portMAX_DELAY);
    (void)ulTaskNotifyValueClear(ts->task, TS_BIT(t->id));
}

/* Let the timer expire `first` ticks from now, then every `cycle` if that is
 * not 0. FreeRTOS has no timer of 0 ticks: such a timeout is set at once, as
 * the reference service reports it with the next poll. */
static void arm(TS_SERVICE_T *ts, TS_TIMER_T *t, TS_TICKS_T first)
{
    if (first == 0U) {
        (void)xTaskNotify(ts->task, TS_BIT(t->id), eSetBits);
        if (t->cycle == 0U) {
            return;
        }
        first = t->cycle;
    }
    t->restoreCycle = (t->cycle != 0U) && (first != t->cycle);
    vTimerSetReloadMode(t->handle, (t->cycle != 0U) ? pdTRUE : pdFALSE);
    (void)xTimerChangePeriod(t->handle, (TickType_t)first, portMAX_DELAY);   /* also starts it */
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
    disarm(ts, t);
    t->cycle = (mode == TS_EVERY) ? ticks : 0U;
    arm(ts, t, ticks);
    return true;
}

void tsStop(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    TS_TIMER_T *t = timerOf(ts, machine, timerId);
    if (t != NULL) {
        disarm(ts, t);
        t->cycle = 0U;
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
    if ((t == NULL) || (xTimerIsTimerActive(t->handle) == pdFALSE)) {
        return 0U;
    }
    return (TS_TICKS_T)(xTimerGetExpiryTime(t->handle) - xTaskGetTickCount());
}

bool tsSetTimeLeft(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T ticks)
{
    TS_TIMER_T *t = timerOf(ts, machine, timerId);
    if (t == NULL) {
        return false;
    }
    if (xTimerIsTimerActive(t->handle) == pdFALSE) {
        /* The timer does not run. A once timer whose bit is still set has
         * expired, but its timeout is not taken yet: start it again, as the
         * reference service takes back a timeout it has not reported.
         * Clearing no bits reads the notification value without changing it. */
        if ((ulTaskNotifyValueClear(ts->task, 0U) & TS_BIT(t->id)) == 0U) {
            return false;
        }
    }
    disarm(ts, t);
    arm(ts, t, ticks);
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
