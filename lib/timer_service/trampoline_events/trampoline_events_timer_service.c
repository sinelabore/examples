/* SPDX-License-Identifier: MIT */
/* Timer service on AUTOSAR OS / OSEK alarms (Trampoline), timeouts as events;
 * see timer_service.h. */
#include "timer_service.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef TS_ERROR_HANDLER_DEFAULT
void tsErrorExit(TS_ERROR_T error, const void *machine, TS_TIMER_ID_T timerId)
{
    (void)fprintf(stderr, "timer service: error %d (machine %p, timer %u)\n",
                  (int)error, machine, (unsigned)timerId);
    exit(EXIT_FAILURE);
}
#endif

/* The service, if it can report a timeout; NULL after reporting why not. */
static const TS_SERVICE_T *reporting(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    if (ts == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_SERVICE, machine, timerId);
        return NULL;
    }
    if (ts->timeout == NULL) {
        TS_ERROR_HANDLER(TS_ERR_NO_CALLBACK, machine, timerId);
        return NULL;
    }
    return ts;
}

/* Called once per activation of the timer id's task. A period whose
 * activation the OS refused (E_OS_LIMIT: the task was still activated) never
 * gets here, so an overrun is dropped without a report (see timer_service.h). */
void tsExpired(const TS_SERVICE_T *ts, TS_TIMER_ID_T timerId)
{
    if (reporting(ts, NULL, timerId) == NULL) {
        return;
    }
    if (timerId >= ts->count) {
        TS_ERROR_HANDLER(TS_ERR_NO_TIMER, NULL, timerId);
        return;
    }
    ts->timeout(ts->context, timerId);
}

/* The timer's alarm, or NULL after reporting why there is none. */
static TS_ALARM_T *alarmOf(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
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
    return &ts->alarms[timerId];
}

/* Stop the alarm. CancelAlarm of an alarm that does not run answers
 * E_OS_NOFUNC: no error. A timeout posted before stays posted. */
static void disarm(const TS_ALARM_T *a)
{
    (void)CancelAlarm(a->alarm);
}

/* Let the alarm expire `first` ticks from now, then every `cycle` if that is
 * not 0. An alarm cannot be set to 0 ticks (E_OS_VALUE): such a timeout is
 * reported at once, in the calling task, as the reference service reports
 * it with the next poll. */
static bool arm(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T first)
{
    const TS_ALARM_T *a = &ts->alarms[timerId];
    StatusType status;
    if (first == 0U) {
        if (reporting(ts, machine, timerId) == NULL) {
            return false;
        }
        ts->timeout(ts->context, timerId);
        if (a->cycle == 0U) {
            return true;
        }
        first = a->cycle;
    }
    status = SetRelAlarm(a->alarm, (TickType)first, (TickType)a->cycle);
    if (status != E_OK) {
        TS_ERROR_HANDLER((status == E_OS_VALUE) ? TS_ERR_TOO_LONG : TS_ERR_OS, machine, timerId);
        return false;
    }
    return true;
}

bool tsStart(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId,
             TS_TICKS_T ticks, TS_MODE_T mode)
{
    TS_ALARM_T *a = alarmOf(ts, machine, timerId);
    if (a == NULL) {
        return false;
    }
    if ((mode == TS_EVERY) && (ticks == 0U)) {
        TS_ERROR_HANDLER(TS_ERR_NO_PERIOD, machine, timerId);
        return false;
    }
    disarm(a);
    a->cycle = (mode == TS_EVERY) ? ticks : 0U;
    return arm(ts, machine, timerId, ticks);
}

void tsStop(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    TS_ALARM_T *a = alarmOf(ts, machine, timerId);
    if (a != NULL) {
        disarm(a);
        a->cycle = 0U;
    }
}

void tsStopAll(TS_SERVICE_T *ts, const void *machine)
{
    TS_TIMER_ID_T id;
    if (alarmOf(ts, machine, 0U) == NULL) {
        return;
    }
    for (id = 0U; id < ts->count; id++) {
        tsStop(ts, machine, id);
    }
}

TS_TICKS_T tsTimeLeft(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId)
{
    const TS_ALARM_T *a = alarmOf(ts, machine, timerId);
    TickType left = 0U;
    if ((a == NULL) || (GetAlarm(a->alarm, &left) != E_OK)) {
        return 0U;                          /* E_OS_NOFUNC: the alarm does not run */
    }
    return (TS_TICKS_T)left;
}

bool tsSetTimeLeft(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T ticks)
{
    TS_ALARM_T *a = alarmOf(ts, machine, timerId);
    TickType left;
    if ((a == NULL) || (GetAlarm(a->alarm, &left) != E_OK)) {
        /* not running; a once timer that expired has posted its timeout,
         * which cannot be taken back */
        return false;
    }
    disarm(a);
    return arm(ts, machine, timerId, ticks);
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
