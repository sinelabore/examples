/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Peter Mueller
 */
/**
 * @file timer_service.h
 * @brief Reference timer service for SinelaboreRT state machines (C99).
 *
 * Polled: the service never calls into a state machine. The application
 * calls tsPoll() regularly, gets the list of timers that expired, and hands
 * each one to the machine that started it (`<machine>SetTimeout()`).
 *
 * No heap, no threads, no interrupts. The timer table is an array the
 * application provides. Every function must be called from the context
 * that also runs the state machines (main loop or one task).
 *
 * Each timer counts its ticks down: tsPoll() takes the time since the last
 * poll off every running timer. The time is a free running tick counter
 * that may wrap; the time between two polls is an unsigned difference, so a
 * wrap is harmless as long as the application polls at least once per
 * counter range. Three settings, which the application may define before
 * including this header (or with `-D`): #TS_TICKS_TYPE, #TS_TICK_US and
 * #TS_TIMER_ID_TYPE.
 *
 * | TS_TICKS_TYPE | TS_TICK_US | Timers, and the time between polls, up to |
 * | ------------- | ---------- | ----------------------------------------- |
 * | uint32_t      | 1000       | 49.7 days (the default)                   |
 * | uint32_t      | 1          | 71.6 minutes                              |
 * | uint64_t      | 1          | no practical limit; costly on 8 and 16 bit parts, and a counter an interrupt updates needs a guarded read |
 *
 * Generated code converts durations with TS_US(), TS_MS() and TS_S().
 *
 * A call that cannot be served is an error, reported to an error handler
 * (see #TS_ERROR_HANDLER): no service, no machine, no table, a full table,
 * an every timer without a period, or a duration too long for the ticks.
 * A timer that does not run is no error: stopping it does nothing, and the
 * functions for the time left answer false or 0, which the generated code
 * relies on.
 */
#ifndef TIMER_SERVICE_H
#define TIMER_SERVICE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Settings
 * Define them before including this header, the same in every file, or with `-D`.
 * @{
 */
#ifndef TS_TICKS_TYPE
/** The type of the tick counter, unsigned; default uint32_t. */
#define TS_TICKS_TYPE uint32_t
#endif
#ifndef TS_TICK_US
/** The length of one tick in microseconds, default 1000. It must divide 1000
 *  (1, 2, 5, 10, ..., 1000); another value does not compile. */
#define TS_TICK_US 1000U
#endif
#ifndef TS_TIMER_ID_TYPE
/** The type of a machine's timer id, default uint16_t. The generator's
 *  `TimerIdType` should name the same type. */
#define TS_TIMER_ID_TYPE uint16_t
#endif
/** @} */

/** Ticks of #TS_TICK_US microseconds. */
typedef TS_TICKS_TYPE TS_TICKS_T;

/** @cond INTERNAL */
/* A tick that does not divide 1000 us fails to compile here. */
typedef char TS_TICK_US_MUST_DIVIDE_1000[((1000U % (TS_TICK_US)) == 0U) ? 1 : -1];
/** @endcond */

/**
 * The largest TS_TICKS_T: the longest time a timer can have left.
 *
 * The outer cast is needed: for a type narrower than int, `~` works on the
 * promoted int and gives -1, and only the cast turns that into the type's
 * maximum.
 */
#define TS_TICKS_MAX ((TS_TICKS_T)~(TS_TICKS_T)0)

/**
 * @name Durations to ticks, for constants
 * TS_US() rounds up, so a timer never fires early; TS_MS() and TS_S()
 * multiply by a whole factor. They compute in uintmax_t and convert only the
 * result, which is right as long as the duration fits TS_TICKS_T; a longer
 * one wraps without notice. The generator therefore checks every literal
 * duration of a model at compile time with TS_US_FITS(), TS_MS_FITS() and
 * TS_S_FITS(), integer constant expressions that are 1 if the duration fits.
 * A duration known only at run time goes through tsUs(), tsMs() or tsS(),
 * which check it.
 * @{
 */
/** @param x a duration in microseconds  @return the ticks, rounded up */
#define TS_US(x)  ((TS_TICKS_T)(((uintmax_t)(x) + ((TS_TICK_US) - 1U)) / (TS_TICK_US)))
/** @param x a duration in milliseconds  @return the ticks */
#define TS_MS(x)  ((TS_TICKS_T)((uintmax_t)(x) * (1000U / (TS_TICK_US))))
/** @param x a duration in seconds  @return the ticks */
#define TS_S(x)   ((TS_TICKS_T)((uintmax_t)(x) * (1000000UL / (TS_TICK_US))))
/** The longest duration in microseconds that fits TS_TICKS_T: #TS_TICKS_MAX
 *  ticks, or all of uintmax_t where that is less. The checks compare with
 *  limits computed at compile time, so a check at run time divides nothing. */
#define TS_US_MAX (((uintmax_t)TS_TICKS_MAX > UINTMAX_MAX / (TS_TICK_US)) \
        ? UINTMAX_MAX : (uintmax_t)TS_TICKS_MAX * (TS_TICK_US))
/** @param x a duration in microseconds  @return 1 if TS_US(x) fits TS_TICKS_T, else 0 */
#define TS_US_FITS(x) ((uintmax_t)(x) <= TS_US_MAX)
/** @param x a duration in milliseconds  @return 1 if TS_MS(x) fits TS_TICKS_T, else 0 */
#define TS_MS_FITS(x) ((uintmax_t)(x) <= (uintmax_t)TS_TICKS_MAX / (1000U / (TS_TICK_US)))
/** @param x a duration in seconds  @return 1 if TS_S(x) fits TS_TICKS_T, else 0 */
#define TS_S_FITS(x)  ((uintmax_t)(x) <= (uintmax_t)TS_TICKS_MAX / (1000000UL / (TS_TICK_US)))
/** The inverse of TS_MS().
 *  @param t ticks  @return whole milliseconds, rounded down */
#define TS_TO_MS(t) ((TS_TICKS_T)((TS_TICKS_T)(t) / (1000U / (TS_TICK_US))))
/** @} */

/** A machine's timer id; see #TS_TIMER_ID_TYPE. */
typedef TS_TIMER_ID_TYPE TS_TIMER_ID_T;

/** How a timer repeats. */
typedef enum {
    TS_ONCE = 0,                /**< after(...): fires once */
    TS_EVERY = 1                /**< every(...): fires until stopped */
} TS_MODE_T;

/**
 * @name Timer states
 * The state of one timer as the machine keeps it, one byte per timer id in
 * the machine's instance data. Generated code sets #TS_RUNNING when the
 * timer's state is entered and #TS_IDLE when it is left;
 * `<machine>SetTimeout()` turns #TS_RUNNING into #TS_EXPIRED, and only
 * #TS_RUNNING, so a timeout reported for a timer the machine has stopped in
 * the meantime is ignored.
 * @{
 */
#define TS_IDLE     0U          /**< not started, stopped, or an after() whose timeout was taken */
#define TS_RUNNING  1U          /**< started; its timeout has not arrived */
#define TS_EXPIRED  2U          /**< its timeout arrived and waits for a step without event */
/** @} */

/** One expired timer, as tsPoll() reports it. */
typedef struct {
    const void *machine;        /**< instance data of the machine that started it */
    TS_TIMER_ID_T timerId;      /**< that machine's timer id */
    uint16_t periods;           /**< how many times it expired since the last report: 1, more for
                                     a #TS_EVERY timer whose periods a late poll missed, i.e. an
                                     overrun. The timeout is handled once; the count lets the
                                     application recognise the overrun if it wants to. */
} TS_TIMEOUT_T;

/** What went wrong, as the error handler is told. */
typedef enum {
    TS_ERR_NO_SERVICE = 1,      /**< ts is NULL, e.g. a machine whose timerService was never set */
    TS_ERR_NO_MACHINE,          /**< machine is NULL */
    TS_ERR_NO_TABLE,            /**< tsInit() without a table, or tsPoll() without out[] */
    TS_ERR_TABLE_FULL,          /**< tsStart() found no free entry: the table is too small */
    TS_ERR_NO_PERIOD,           /**< tsStart() of a #TS_EVERY timer with 0 ticks */
    TS_ERR_TOO_LONG             /**< tsUs(), tsMs() or tsS(): the duration does not fit TS_TICKS_T */
} TS_ERROR_T;

#ifndef TS_ERROR_HANDLER
/**
 * The name of the error handler, default tsErrorExit.
 *
 * Name your own with `-DTS_ERROR_HANDLER=myHandler` (or define it before
 * including this header, the same in every file), with the parameters of
 * the declaration below. Without it the service calls tsErrorExit(), which
 * prints the error to stderr and ends the program with
 * `exit(EXIT_FAILURE)`; it is compiled only then, so a target without
 * exit() needs a handler of its own.
 */
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

/** One entry of the timer table. The application provides the table, an
 *  array of these; the service fills it. */
typedef struct {
    const void *machine;        /**< NULL: entry is free; only compared, never written through */
    TS_TIMER_ID_T timerId;      /**< the machine's timer id */
    bool every;                 /**< #TS_EVERY: goes on with its period after expiring */
    bool pending;               /**< expired, but tsPoll() had no room in out[] for it yet */
    uint16_t periods;           /**< expiries not reported yet; more than 1 after a late poll */
    TS_TICKS_T period;          /**< a #TS_EVERY timer's period */
    TS_TICKS_T left;            /**< ticks until it expires, counted from the last tsPoll() */
} TS_TIMER_T;

/** The service: the application's timer table and the time of the last
 *  poll. Set up with tsInit(); the application does not write it. */
typedef struct TS_SERVICE {
    TS_TIMER_T *table;          /**< the application's timer table */
    uint16_t capacity;          /**< its number of entries */
    TS_TICKS_T now;             /**< the time given to tsInit() or the last tsPoll() */
    uint16_t refused;           /**< starts refused because every entry was in use (#TS_ERR_TABLE_FULL) */
} TS_SERVICE_T;

/**
 * @name The application
 * @{
 */

/**
 * Prepare the service with the application's timer table. Every entry is
 * marked free.
 *
 * @param[out] ts        the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in]  table     the timer table, @p capacity entries; NULL with a
 *                       capacity above 0 is #TS_ERR_NO_TABLE, and if the
 *                       handler returns, the service has no entries
 * @param[in]  capacity  the number of entries in @p table
 * @param[in]  now       the current time; the first tsPoll() counts from it
 */
void tsInit(TS_SERVICE_T *ts, TS_TIMER_T *table, uint16_t capacity, TS_TICKS_T now);

/**
 * Count the running timers down by the time since the last poll, and
 * report the ones that expired in out[], one entry per timer.
 *
 * A #TS_ONCE timer is removed once reported, a #TS_EVERY timer goes on with
 * its next period. A #TS_EVERY timer that a late poll finds several periods
 * on still takes one entry, with TS_TIMEOUT_T::periods above 1 (an
 * overrun, for the application to check if it wants to). A timer
 * started afterwards counts from @p now.
 *
 * @param[in,out] ts   the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in]     now  the current time
 * @param[out]    out  receives the expired timers; NULL with @p max above 0
 *                     is #TS_ERR_NO_TABLE, and if the handler returns, the
 *                     timers are still counted down and none is reported
 * @param[in]     max  the number of entries @p out has room for
 * @return n, the number of entries written to out[0] .. out[n-1]: the
 *         number of timers reported, not of periods. It is at most @p max.
 *         When it equals @p max, more timers may have expired; they stay
 *         expired and the next call reports them (a call with the same
 *         @p now does so at once).
 * @retval 0 nothing expired, or @p ts or @p out is NULL (after the error
 *           handler)
 */
uint16_t tsPoll(TS_SERVICE_T *ts, TS_TICKS_T now, TS_TIMEOUT_T *out, uint16_t max);

/**
 * The number of running timers.
 *
 * @param[in] ts  the service; NULL is #TS_ERR_NO_SERVICE
 * @return the entries in use, of all machines; 0 if @p ts is NULL
 */
uint16_t tsRunning(const TS_SERVICE_T *ts);

/** @} */

/**
 * @name Start and stop: needed by every machine with a timer
 * The generated entry code starts a state's timers, the exit code stops
 * them, and the reset stops all of the machine's timers.
 * @{
 */

/**
 * Start (or restart) the timer (machine, timerId). It expires @p ticks after
 * the time given to the last tsPoll().
 *
 * @param[in,out] ts       the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in]     machine  the machine's instance data, which identifies it;
 *                         NULL is #TS_ERR_NO_MACHINE
 * @param[in]     timerId  the machine's timer id
 * @param[in]     ticks    the time until it expires, and a #TS_EVERY timer's
 *                         period; 0 with #TS_EVERY is #TS_ERR_NO_PERIOD
 * @param[in]     mode     #TS_ONCE or #TS_EVERY
 * @retval true   started
 * @retval false  refused: an error above, or a full table
 *                (#TS_ERR_TABLE_FULL, counted in TS_SERVICE_T::refused)
 */
bool tsStart(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId,
             TS_TICKS_T ticks, TS_MODE_T mode);

/**
 * Stop the timer (machine, timerId). Stopping a timer that is not running
 * does nothing.
 *
 * @param[in,out] ts       the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in]     machine  the machine's instance data; NULL is #TS_ERR_NO_MACHINE
 * @param[in]     timerId  the machine's timer id
 */
void tsStop(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId);

/**
 * Stop every timer of one machine, e.g. before its instance goes away.
 *
 * @param[in,out] ts       the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in]     machine  the machine's instance data; NULL is #TS_ERR_NO_MACHINE
 */
void tsStopAll(TS_SERVICE_T *ts, const void *machine);

/** @} */

/**
 * @name The time left: needed only for TIME_LEFT and the commands of named timers
 * The generated code calls these only for a model that reads or changes a
 * running time: `TIME_LEFT(name)` and the commands set, increment and
 * decrement of named timers, each of them one tsSetTimeLeft(); the model's
 * own code may call them too. A service built on other timers needs them
 * only for such models. Times count from the last tsPoll(). A timeout
 * already handed to the machine stays with it; one that expired but was not
 * reported yet is taken back by setting the time left. A #TS_ONCE timer is
 * gone once tsPoll() has reported it.
 * @{
 */

/**
 * The ticks until the timer (machine, timerId) expires.
 *
 * @param[in] ts       the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in] machine  the machine's instance data; NULL is #TS_ERR_NO_MACHINE
 * @param[in] timerId  the machine's timer id
 * @return the ticks left, counted from the last tsPoll(); 0 when the timer
 *         does not run or has expired
 */
TS_TICKS_T tsTimeLeft(const TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId);

/**
 * Let the timer expire @p ticks from now, e.g. to lengthen or shorten a
 * cooking time while it runs. A #TS_EVERY timer keeps its period from then on.
 *
 * @param[in,out] ts       the service; NULL is #TS_ERR_NO_SERVICE
 * @param[in]     machine  the machine's instance data; NULL is #TS_ERR_NO_MACHINE
 * @param[in]     timerId  the machine's timer id
 * @param[in]     ticks    the new time left, counted from the last tsPoll();
 *                         with 0 it expires now, and the next tsPoll()
 *                         reports its timeout
 * @retval true   done
 * @retval false  the timer does not run, or an error above
 */
bool tsSetTimeLeft(TS_SERVICE_T *ts, const void *machine, TS_TIMER_ID_T timerId, TS_TICKS_T ticks);

/** @} */

/**
 * @name Durations to ticks, at run time
 * For values known only at run time, such as `after(cookMs ms)`: as TS_US(),
 * TS_MS() and TS_S(), but a duration that does not fit TS_TICKS_T is the
 * error #TS_ERR_TOO_LONG; if the handler returns, the result is
 * #TS_TICKS_MAX. A negative value converts to a very large one and is
 * reported the same way. The generated code uses these for durations given
 * as expressions.
 * @{
 */

/** @param us a duration in microseconds  @return the ticks, rounded up */
TS_TICKS_T tsUs(uintmax_t us);
/** @param ms a duration in milliseconds  @return the ticks */
TS_TICKS_T tsMs(uintmax_t ms);
/** @param s a duration in seconds  @return the ticks */
TS_TICKS_T tsS(uintmax_t s);

/** @} */

/**
 * @name Arithmetic on times
 * For the commands increment and decrement of named timers, on the time
 * left of a running timer and on the time a named timer keeps while it does
 * not run.
 * @{
 */

/** @param time a time  @param ticks the ticks to add
 *  @return time + ticks, at most #TS_TICKS_MAX */
TS_TICKS_T tsTimeAdd(TS_TICKS_T time, TS_TICKS_T ticks);
/** @param time a time  @param ticks the ticks to take off
 *  @return time - ticks, at least 0 */
TS_TICKS_T tsTimeSub(TS_TICKS_T time, TS_TICKS_T ticks);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* TIMER_SERVICE_H */
