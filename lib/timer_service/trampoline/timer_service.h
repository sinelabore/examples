/* SPDX-License-Identifier: MIT */
/**
 * @file trampoline/timer_service.h
 * @brief Timer service for SinelaboreRT state machines on AUTOSAR OS / OSEK
 *        alarms, here Trampoline (https://github.com/TrampolineRTOS/trampoline).
 *
 * It declares what the generated C code uses from the reference service,
 * ../timer_service.h, with the same names and meanings, so the machine
 * compiles unchanged against it: the timer states, the modes, the tick
 * type, the duration macros, and tsStart(), tsStop(), tsStopAll(),
 * tsTimeLeft(), tsSetTimeLeft(), tsTimeAdd(), tsTimeSub().
 * There is no tsPoll(): the OS reports a timeout as an event.
 *
 * Each timer id of a machine has an alarm and an event of its own, declared
 * in the OIL file. The alarm sets the event in the task that runs the
 * machine; the task waits for its events and hands each timeout to the
 * machine (`<machine>SetTimeout()`), which takes it in a step without event.
 *
 * | Function        | OS calls |
 * | --------------- | -------- |
 * | tsStart()       | CancelAlarm, ClearEvent, SetRelAlarm(ticks, cycle) |
 * | tsStop()        | CancelAlarm, ClearEvent |
 * | tsTimeLeft()    | GetAlarm |
 * | tsSetTimeLeft() | CancelAlarm, ClearEvent, SetRelAlarm(ticks, the same cycle) |
 *
 * ClearEvent drops a timeout that is set but not yet taken, so a stopped or
 * restarted timer cannot expire early. ClearEvent works only on the calling
 * task's own events: every call comes from the task named in TS_SERVICE_T,
 * which is where the generated code runs anyway.
 */
#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H

#include <stdbool.h>
#include <stdint.h>
#include "tpl_os.h"

#ifndef TS_TICK_US
/** One tick of the OS counter the alarms use, in microseconds. Trampoline's
 *  POSIX target counts SystemCounter every 10 ms (the ViPER signal). */
#define TS_TICK_US 10000U
#endif

typedef uint32_t TS_TICKS_T;            /**< counter ticks */
/** The largest TS_TICKS_T: the longest time a timer can have left. */
#define TS_TICKS_MAX ((TS_TICKS_T)0xFFFFFFFFU)

typedef uint16_t TS_TIMER_ID_T;         /**< a machine's timer id; see `TimerIdType` */

/** How a timer repeats. */
typedef enum {
    TS_ONCE = 0,                /**< after(...): fires once */
    TS_EVERY = 1                /**< every(...): fires until stopped */
} TS_MODE_T;

/**
 * @name Timer states
 * The state of one timer as the machine keeps it (see the reference header).
 * @{
 */
#define TS_IDLE     0U          /**< not started, stopped, or an after() whose timeout was taken */
#define TS_RUNNING  1U          /**< started; its timeout has not arrived */
#define TS_EXPIRED  2U          /**< its timeout arrived and waits for a step without event */
/** @} */

/**
 * @name Durations to ticks, for constants
 * Rounded up so a timer never fires early: with 10 ms ticks, TS_MS(15U) is
 * 2 ticks. The ..._FITS macros tell at compile time whether a literal fits
 * TS_TICKS_T; the generated header checks each one.
 * @{
 */
/** @param x a duration in microseconds  @return the ticks, rounded up */
#define TS_US(x) ((TS_TICKS_T)(((uintmax_t)(x) + (TS_TICK_US - 1U)) / TS_TICK_US))
/** @param x a duration in milliseconds  @return the ticks, rounded up */
#define TS_MS(x) ((TS_TICKS_T)(((uintmax_t)(x) * 1000U + (TS_TICK_US - 1U)) / TS_TICK_US))
/** @param x a duration in seconds  @return the ticks, rounded up */
#define TS_S(x)  ((TS_TICKS_T)(((uintmax_t)(x) * 1000000U + (TS_TICK_US - 1U)) / TS_TICK_US))
/** @param x a duration in microseconds  @return 1 if TS_US(x) fits TS_TICKS_T, else 0 */
#define TS_US_FITS(x) ((((uintmax_t)(x) + (TS_TICK_US - 1U)) / TS_TICK_US) <= TS_TICKS_MAX)
/** @param x a duration in milliseconds  @return 1 if TS_MS(x) fits TS_TICKS_T, else 0 */
#define TS_MS_FITS(x) (((uintmax_t)(x) <= UINTMAX_MAX / 1000U) && \
                       ((((uintmax_t)(x) * 1000U + (TS_TICK_US - 1U)) / TS_TICK_US) <= TS_TICKS_MAX))
/** @param x a duration in seconds  @return 1 if TS_S(x) fits TS_TICKS_T, else 0 */
#define TS_S_FITS(x)  (((uintmax_t)(x) <= UINTMAX_MAX / 1000000U) && \
                       ((((uintmax_t)(x) * 1000000U + (TS_TICK_US - 1U)) / TS_TICK_US) <= TS_TICKS_MAX))
/** @param t ticks  @return whole milliseconds, rounded down */
#define TS_TO_MS(t) ((uintmax_t)(t) * TS_TICK_US / 1000U)
/** @} */

/** Errors, reported to #TS_ERROR_HANDLER (default: print and exit). */
typedef enum {
    TS_ERR_NO_SERVICE = 1,      /**< ts is NULL: the machine's timerService was never set */
    TS_ERR_NO_MACHINE,          /**< machine is NULL */
    TS_ERR_NO_TIMER,            /**< a timer id beyond the service's alarms */
    TS_ERR_NO_PERIOD,           /**< tsStart() of a #TS_EVERY timer with 0 ticks */
    TS_ERR_TOO_LONG,            /**< a duration the counter cannot reach (SetRelAlarm: E_OS_VALUE) */
    TS_ERR_OS                   /**< another error of the OS */
} TS_ERROR_T;

#ifndef TS_ERROR_HANDLER
/** The name of the error handler, default tsErrorExit, which prints the
 *  error and exits. Name your own with `-DTS_ERROR_HANDLER=myHandler`. */
#define TS_ERROR_HANDLER tsErrorExit
/** @cond INTERNAL */
#define TS_ERROR_HANDLER_DEFAULT
/** @endcond */
#endif
/**
 * The error handler, named by #TS_ERROR_HANDLER. If it returns, the call
 * that failed does nothing and returns false or 0.
 *
 * @param error    what went wrong
 * @param machine  the machine of the call that failed; NULL where it has none
 * @param timerId  the timer id of the call that failed; 0 where it has none
 */
void TS_ERROR_HANDLER(TS_ERROR_T error, const void *machine, TS_TIMER_ID_T timerId);

/** The alarm and event of one timer id. */
typedef struct {
    AlarmType alarm;            /**< the alarm, from the OIL file */
    EventMaskType event;        /**< the event it sets in the machine's task */
    TS_TICKS_T cycle;           /**< kept by the service: the period of a #TS_EVERY timer, 0 for #TS_ONCE */
} TS_ALARM_T;

/** The service of one machine: the task that runs it, and an alarm and event
 *  per timer id, indexed by the id. The application fills it; there is no
 *  tsInit(). */
typedef struct TS_SERVICE {
    TaskType task;              /**< the task that runs the machine */
    TS_ALARM_T *alarms;         /**< an alarm and event per timer id */
    uint16_t count;             /**< the number of timer ids */
} TS_SERVICE_T;

/**
 * @name What the generated code calls, always from ts->task
 * Each function reports a NULL @p ts (#TS_ERR_NO_SERVICE), a NULL machine
 * (#TS_ERR_NO_MACHINE) and a timer id beyond the service's
 * (#TS_ERR_NO_TIMER). The service serves one machine: machine is only
 * checked, not used to find the alarm.
 * @{
 */

/**
 * Start (or restart) the timer to expire @p ticks from now.
 *
 * @param[in,out] ts       the machine's service
 * @param[in]     machine  the machine's instance data
 * @param[in]     timerId  the machine's timer id
 * @param[in]     ticks    the time until it expires, and a #TS_EVERY timer's
 *                         period; 0 expires at once, and with #TS_EVERY is
 *                         #TS_ERR_NO_PERIOD
 * @param[in]     mode     #TS_ONCE or #TS_EVERY
 * @retval true   started
 * @retval false  refused: an error above, or SetRelAlarm failed
 *                (#TS_ERR_TOO_LONG, #TS_ERR_OS)
 */
bool tsStart(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId,
             TS_TICKS_T ticks, TS_MODE_T mode);

/**
 * Stop the timer; a timeout already set but not taken is dropped.
 *
 * @param[in,out] ts       the machine's service
 * @param[in]     machine  the machine's instance data
 * @param[in]     timerId  the machine's timer id
 */
void tsStop(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId);

/**
 * Stop every timer of the machine.
 *
 * @param[in,out] ts       the machine's service
 * @param[in]     machine  the machine's instance data
 */
void tsStopAll(TS_SERVICE_T *ts, const void *machine);

/**
 * The ticks until the timer expires.
 *
 * @param[in] ts       the machine's service
 * @param[in] machine  the machine's instance data
 * @param[in] timerId  the machine's timer id
 * @return the ticks left; 0 when the timer does not run or has expired
 */
TS_TICKS_T tsTimeLeft(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId);

/**
 * Let the timer expire @p ticks from now. A #TS_EVERY timer keeps its period.
 * A once timer that has expired but whose timeout is not taken yet is
 * started again, its timeout dropped.
 *
 * @param[in,out] ts       the machine's service
 * @param[in]     machine  the machine's instance data
 * @param[in]     timerId  the machine's timer id
 * @param[in]     ticks    the new time left; 0 means at once
 * @retval true   done
 * @retval false  the timer does not run, an error above, or SetRelAlarm
 *                failed (#TS_ERR_TOO_LONG, #TS_ERR_OS)
 */
bool tsSetTimeLeft(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T ticks);

/** @} */

/**
 * @name Durations to ticks, at run time
 * For after(x ms) with an expression. A duration that does not fit
 * TS_TICKS_T is #TS_ERR_TOO_LONG; if the handler returns, the result is
 * #TS_TICKS_MAX.
 * @{
 */
/** @param us a duration in microseconds  @return the ticks, rounded up */
TS_TICKS_T tsUs(uintmax_t us);
/** @param ms a duration in milliseconds  @return the ticks, rounded up */
TS_TICKS_T tsMs(uintmax_t ms);
/** @param s a duration in seconds  @return the ticks, rounded up */
TS_TICKS_T tsS(uintmax_t s);
/** @} */

/**
 * @name Arithmetic on times
 * For increment and decrement of named timers.
 * @{
 */
/** @param time a time  @param ticks the ticks to add
 *  @return time + ticks, at most #TS_TICKS_MAX */
TS_TICKS_T tsTimeAdd(TS_TICKS_T time, TS_TICKS_T ticks);
/** @param time a time  @param ticks the ticks to take off
 *  @return time - ticks, at least 0 */
TS_TICKS_T tsTimeSub(TS_TICKS_T time, TS_TICKS_T ticks);
/** @} */

#endif /* TIMER_SERVICE_H */
