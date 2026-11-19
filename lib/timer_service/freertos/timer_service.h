/* SPDX-License-Identifier: MIT */
/**
 * @file freertos/timer_service.h
 * @brief Timer service for SinelaboreRT state machines on FreeRTOS software timers.
 *
 * It declares what the generated C code uses from the reference service,
 * ../timer_service.h, with the same names and meanings, so the machine
 * compiles unchanged against it: the timer states, the modes, the tick
 * type, the duration macros, and tsStart(), tsStop(), tsStopAll(),
 * tsTimeLeft(), tsSetTimeLeft(), tsTimeAdd(), tsTimeSub().
 * There is no tsPoll(): a timeout arrives as a task notification.
 *
 * Each timer id of a machine has a FreeRTOS software timer and a notification
 * bit of the task that runs the machine, bit `id`. The timer's callback sets
 * the bit; the task waits for its bits and hands each timeout to the machine
 * (`<machine>SetTimeout()`), which takes it in a step without event.
 *
 * | Function        | FreeRTOS calls |
 * | --------------- | -------------- |
 * | tsStart()       | xTimerStop, clear the bit, xTimerChangePeriod (once or reload) |
 * | tsStop()        | xTimerStop, clear the bit |
 * | tsTimeLeft()    | xTimerGetExpiryTime - now |
 * | tsSetTimeLeft() | xTimerStop, clear the bit, xTimerChangePeriod; an every timer gets its period back in the callback |
 *
 * Clearing the bit drops a timeout that is set but not yet taken, so a
 * stopped or restarted timer cannot expire early.
 *
 * Requirements: the machine runs in one task, the one in TS_SERVICE_T, and
 * every call except the timer callback comes from it; the timer task has the
 * highest priority, so a timer command is carried out before the call that
 * sent it returns; tsInit() runs before the scheduler starts.
 */
#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#ifndef TS_TICK_US
/** One tick in microseconds: the FreeRTOS tick. `configTICK_RATE_HZ` must
 *  divide 1000000 (freertos_timer_service.c checks it). */
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
#define TS_RUNNING  1U          /**< started; its timeout has not arrived */
#define TS_EXPIRED  2U          /**< its timeout arrived and waits for a step without event */
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
    TS_ERR_OS                   /**< FreeRTOS could not create a timer */
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

struct TS_SERVICE;

/** One timer of the machine; filled by tsInit(). */
typedef struct {
    struct TS_SERVICE *service;         /**< the service it belongs to */
    TimerHandle_t handle;               /**< its FreeRTOS software timer */
    TS_TIMER_ID_T id;                   /**< its timer id, and its notification bit */
    TS_TICKS_T cycle;                   /**< the period of a #TS_EVERY timer, 0 for #TS_ONCE */
    bool restoreCycle;                  /**< the next expiry goes back to `cycle` */
} TS_TIMER_T;

/** The service of one machine: the task that runs it, and a timer per timer
 *  id, indexed by the id. Timer id n notifies bit n of the task, so the task
 *  keeps bits 0 to count - 1 for the service. */
typedef struct TS_SERVICE {
    TaskHandle_t task;                  /**< the task that runs the machine */
    TS_TIMER_T *timers;                 /**< a timer per timer id */
    uint16_t count;                     /**< the number of timer ids */
} TS_SERVICE_T;

/** The notification bit of a timer id.
 *  @param id a timer id  @return its bit in the task's notification value */
#define TS_BIT(id) ((uint32_t)1U << (id))

/**
 * Create the FreeRTOS timers, once, before vTaskStartScheduler().
 *
 * @param[out] ts      the service of one machine
 * @param[in]  task    the task that runs the machine
 * @param[out] timers  @p count timers, one per timer id
 * @param[in]  count   the number of timer ids, at most 24: notification bits
 *                     24 to 31 stay free for the application. A timer
 *                     FreeRTOS cannot create is #TS_ERR_OS.
 */
void tsInit(TS_SERVICE_T *ts, TaskHandle_t task, TS_TIMER_T *timers, uint16_t count);

/**
 * @name What the generated code calls, always from ts->task
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
 *                         period; 0 expires at once, and with #TS_EVERY is
 *                         #TS_ERR_NO_PERIOD
 * @param[in]     mode     #TS_ONCE or #TS_EVERY
 * @retval true   started
 * @retval false  refused: an error above
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
