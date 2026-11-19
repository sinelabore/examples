/* SPDX-License-Identifier: MIT */
/**
 * @file freertos_events/timer_service.h
 * @brief Timer service for SinelaboreRT state machines on FreeRTOS software
 *        timers, for timeouts as events (`TimeoutsAsEvents=yes`).
 *
 * It declares what the generated C code uses from the reference service,
 * ../timer_service.h, with the same names and meanings, so the machine
 * compiles unchanged against it: the timer states, the modes, the tick
 * type, the duration macros, and tsStart(), tsStop(), tsStopAll(),
 * tsTimeLeft(), tsSetTimeLeft(), tsTimeAdd(), tsTimeSub().
 * There is no tsPoll(): the service reports each timeout to a function of
 * the application as it happens.
 *
 * Each timer id of a machine has a FreeRTOS software timer. Its callback
 * calls the application's #TS_TIMEOUT_FN with the timer id, once per expiry:
 * an every timer once per period. The application looks up the timer's event
 * (`<machine>TimeoutEvent()`) and posts it into the queue of the task that
 * runs the machine, as it posts any other event. The machine task waits on
 * that one queue.
 *
 * | Function        | FreeRTOS calls |
 * | --------------- | -------------- |
 * | tsStart()       | xTimerStop, xTimerChangePeriod (once or reload) |
 * | tsStop()        | xTimerStop |
 * | tsTimeLeft()    | xTimerGetExpiryTime - now |
 * | tsSetTimeLeft() | xTimerStop, xTimerChangePeriod; an every timer gets its period back in the callback |
 *
 * A timeout already posted stays in the queue when its timer is stopped:
 * the generated code ignores the event of a timer that no longer runs (the
 * stale check). A timer restarted before its old event is dispatched takes
 * that event early; the machine task should therefore take its queue
 * promptly.
 *
 * Overrun: when the timer task runs late, e.g. held up for longer than a
 * period, FreeRTOS calls an auto-reload timer's callback once for every
 * period it missed, one after the other (prvReloadTimer() in timers.c). This
 * service passes each call on, so the application receives a burst of
 * timeouts, one per missed period. It neither merges them nor reports the
 * overrun. In a system dimensioned for its fastest timer this does not
 * happen; where it can, the application has to recognise the burst itself.
 *
 * Requirements: every call except the timer callback comes from the task
 * that runs the machine; the timer task has the highest priority, so a
 * timer command is carried out before the call that sent it returns;
 * tsInit() runs before the scheduler starts.
 *
 * The flag mode service is ../freertos; it sets a notification bit instead.
 */
#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "timers.h"

#ifndef TS_TICK_US
/** One tick in microseconds: the FreeRTOS tick. `configTICK_RATE_HZ` must
 *  divide 1000000 (freertos_events_timer_service.c checks it). */
#define TS_TICK_US (1000000U / configTICK_RATE_HZ)
#endif

typedef uint32_t TS_TICKS_T;            /**< ticks */
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
#define TS_RUNNING  1U          /**< started; its timeout has not been taken */
#define TS_EXPIRED  2U          /**< flag mode only */
/** @} */

/**
 * @name Durations to ticks, for constants
 * Rounded up so a timer never fires early. The ..._FITS macros tell at
 * compile time whether a literal fits TS_TICKS_T; the generated header
 * checks each one.
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
    TS_ERR_NO_TIMER,            /**< a timer id beyond the service's timers */
    TS_ERR_NO_PERIOD,           /**< tsStart() of a #TS_EVERY timer with 0 ticks */
    TS_ERR_TOO_LONG,            /**< a duration that does not fit TS_TICKS_T */
    TS_ERR_OS,                  /**< FreeRTOS could not create a timer */
    TS_ERR_NO_CALLBACK          /**< tsInit() without a #TS_TIMEOUT_FN */
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

/**
 * The application's function for a timeout: post the timer's event to the
 * machine. Called once per expiry, an every timer once per period (on
 * overrun, a late timer task calls it once per missed period; see above).
 *
 * It runs in the timer task, or, for a timer started or set to 0 ticks, in
 * the task that started it, i.e. the machine's task. It must not block:
 * post with a wait of 0 (`xQueueSendToBack(queue, &msg, 0)`).
 *
 * @param context  the context given to tsInit(), e.g. the queue
 * @param timerId  the machine's timer id that expired
 */
typedef void (*TS_TIMEOUT_FN)(void *context, TS_TIMER_ID_T timerId);

struct TS_SERVICE;

/** One timer of the machine; filled by tsInit(). */
typedef struct {
    struct TS_SERVICE *service;         /**< the service it belongs to */
    TimerHandle_t handle;               /**< its FreeRTOS software timer */
    TS_TIMER_ID_T id;                   /**< its timer id */
    TS_TICKS_T cycle;                   /**< the period of a #TS_EVERY timer, 0 for #TS_ONCE */
    bool restoreCycle;                  /**< the next expiry goes back to `cycle` */
} TS_TIMER_T;

/** The service of one machine: where timeouts go, and a timer per timer id,
 *  indexed by the id. */
typedef struct TS_SERVICE {
    TS_TIMEOUT_FN timeout;              /**< the application's function for a timeout */
    void *context;                      /**< passed to it, e.g. the machine task's queue */
    TS_TIMER_T *timers;                 /**< a timer per timer id */
    uint16_t count;                     /**< the number of timer ids */
} TS_SERVICE_T;

/**
 * Create the FreeRTOS timers, once, before vTaskStartScheduler().
 *
 * @param[out] ts       the service of one machine
 * @param[in]  timeout  the application's function for a timeout; NULL is
 *                      #TS_ERR_NO_CALLBACK
 * @param[in]  context  passed to @p timeout, e.g. the machine task's queue
 * @param[out] timers   @p count timers, one per timer id
 * @param[in]  count    the number of timer ids. A timer FreeRTOS cannot
 *                      create is #TS_ERR_OS.
 */
void tsInit(TS_SERVICE_T *ts, TS_TIMEOUT_FN timeout, void *context, TS_TIMER_T *timers, uint16_t count);

/**
 * @name What the generated code calls, always from the machine's task
 * Each function reports a NULL @p ts (#TS_ERR_NO_SERVICE), a NULL machine
 * (#TS_ERR_NO_MACHINE) and a timer id beyond the service's
 * (#TS_ERR_NO_TIMER). The service serves one machine: machine is only
 * checked, not used to find the timer.
 * @{
 */

/**
 * Start (or restart) the timer to expire @p ticks from now.
 *
 * @param[in,out] ts       the machine's service
 * @param[in]     machine  the machine's instance data
 * @param[in]     timerId  the machine's timer id
 * @param[in]     ticks    the time until it expires, and a #TS_EVERY timer's
 *                         period; 0 reports the timeout at once, and with
 *                         #TS_EVERY is #TS_ERR_NO_PERIOD
 * @param[in]     mode     #TS_ONCE or #TS_EVERY
 * @retval true   started
 * @retval false  refused: an error above
 */
bool tsStart(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId,
             TS_TICKS_T ticks, TS_MODE_T mode);

/**
 * Stop the timer. A timeout already posted stays posted; the machine
 * ignores it, as the timer no longer runs.
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
 * Let a running timer expire @p ticks from now. A #TS_EVERY timer keeps its
 * period. A once timer that has expired cannot be set: its timeout has been
 * posted already.
 *
 * @param[in,out] ts       the machine's service
 * @param[in]     machine  the machine's instance data
 * @param[in]     timerId  the machine's timer id
 * @param[in]     ticks    the new time left; 0 reports the timeout at once
 * @retval true   done
 * @retval false  the timer does not run, or an error above
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
