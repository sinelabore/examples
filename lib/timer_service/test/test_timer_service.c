/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Peter Mueller
 */
/* Checks of the reference timer service; `make test`. */
#include <assert.h>
#include <stdio.h>
#include "timer_service.h"

static TS_TIMER_T table[3];
static TS_SERVICE_T ts;
static TS_TIMEOUT_T out[3];
static int m1, m2;                  /* two machines; only their addresses count */

/* The error handler of these checks (-DTS_ERROR_HANDLER=testError): it
 * records the error and returns, so the failed call can be checked too. */
static int errors;
static TS_ERROR_T lastError;
void testError(TS_ERROR_T error, const void *machine, TS_TIMER_ID_T timerId)
{
    (void)machine;
    (void)timerId;
    errors++;
    lastError = error;
}

#define TS_TICKS_HALF ((TS_TICKS_T)((TS_TICKS_T)1 << ((sizeof(TS_TICKS_T) * 8U) - 1U)))

int main(void)
{
    /* once: fires at its deadline, once */
    tsInit(&ts, table, 3U, 0U);
    assert(tsStart(&ts, &m1, 7U, 100U, TS_ONCE));
    assert(tsPoll(&ts, 99U, out, 3U) == 0U);
    assert(tsPoll(&ts, 100U, out, 3U) == 1U && out[0].machine == &m1 && out[0].timerId == 7U);
    assert(out[0].periods == 1U);
    assert(tsPoll(&ts, 1000U, out, 3U) == 0U && tsRunning(&ts) == 0U);

    /* across the wrap of the clock */
    tsInit(&ts, table, 3U, (TS_TICKS_T)((TS_TICKS_T)0 - 0x100U));
    (void)tsStart(&ts, &m1, 1U, 0x200U, TS_ONCE);           /* due 0x100 */
    assert(tsPoll(&ts, (TS_TICKS_T)((TS_TICKS_T)0 - 1U), out, 3U) == 0U);
    assert(tsPoll(&ts, 0x0FFU, out, 3U) == 0U);
    assert(tsPoll(&ts, 0x100U, out, 3U) == 1U);

    /* every: keeps its phase; a late poll reports the missed periods once, and counts them */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_EVERY);
    assert(tsPoll(&ts, 350U, out, 3U) == 1U);                /* 100, 200, 300 in one report */
    assert(out[0].periods == 3U);                            /* timeouts as events: 3 events */
    assert(tsPoll(&ts, 399U, out, 3U) == 0U);
    assert(tsPoll(&ts, 400U, out, 3U) == 1U && tsRunning(&ts) == 1U);
    assert(out[0].periods == 1U);
    assert(!tsStart(&ts, &m1, 2U, 0U, TS_EVERY) && lastError == TS_ERR_NO_PERIOD);   /* period 0 */
    errors = 0;

    /* restart: counts again from the current time, still one entry */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);
    assert(tsPoll(&ts, 50U, out, 3U) == 0U);                 /* the time is now 50 */
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);              /* due 150 */
    assert(tsRunning(&ts) == 1U);
    assert(tsPoll(&ts, 100U, out, 3U) == 0U);
    assert(tsPoll(&ts, 150U, out, 3U) == 1U);

    /* the same id in two machines are two timers; stop and stopAll */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);
    (void)tsStart(&ts, &m2, 1U, 100U, TS_ONCE);
    (void)tsStart(&ts, &m2, 2U, 100U, TS_ONCE);
    tsStop(&ts, &m1, 1U);
    tsStop(&ts, &m1, 1U);                                    /* not running: no effect */
    assert(tsRunning(&ts) == 2U);
    tsStopAll(&ts, &m2);
    assert(tsRunning(&ts) == 0U && tsPoll(&ts, 200U, out, 3U) == 0U);

    /* a full table refuses and counts */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 10U, TS_ONCE);
    (void)tsStart(&ts, &m1, 2U, 20U, TS_ONCE);
    (void)tsStart(&ts, &m1, 3U, 30U, TS_ONCE);
    assert(!tsStart(&ts, &m1, 4U, 40U, TS_ONCE) && ts.refused == 1U);
    assert(errors == 1 && lastError == TS_ERR_TABLE_FULL);
    errors = 0;

    /* the time left counts from the last poll */
    assert(tsTimeLeft(&ts, &m1, 2U) == 20U);
    (void)tsPoll(&ts, 15U, out, 3U);
    assert(tsTimeLeft(&ts, &m1, 2U) == 5U);

    /* more expired than out[] holds: the rest comes with the next poll */
    assert(tsPoll(&ts, 100U, out, 1U) == 1U);
    assert(tsPoll(&ts, 100U, out, 3U) == 1U);
    assert(tsRunning(&ts) == 0U);

    /* time left, and changing it while the timer runs */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);              /* due 100 */
    assert(tsTimeLeft(&ts, &m1, 1U) == 100U);
    assert(tsTimeLeft(&ts, &m1, 2U) == 0U && tsTimeLeft(&ts, &m2, 1U) == 0U);
    assert(tsPoll(&ts, 40U, out, 3U) == 0U && tsTimeLeft(&ts, &m1, 1U) == 60U);
    assert(tsSetTimeLeft(&ts, &m1, 1U, 160U));         /* longer: due 200 */
    assert(tsPoll(&ts, 199U, out, 3U) == 0U && tsTimeLeft(&ts, &m1, 1U) == 1U);
    assert(tsSetTimeLeft(&ts, &m1, 1U, 0U));           /* now: due at the next poll */
    assert(tsTimeLeft(&ts, &m1, 1U) == 0U);
    assert(tsPoll(&ts, 199U, out, 3U) == 1U && out[0].timerId == 1U);
    assert(!tsSetTimeLeft(&ts, &m1, 1U, 10U));          /* gone: not restarted */
    assert(tsRunning(&ts) == 0U && tsTimeLeft(&ts, &m1, 1U) == 0U);
    (void)tsStart(&ts, &m1, 2U, 50U, TS_EVERY);              /* due 249 */
    assert(tsSetTimeLeft(&ts, &m1, 2U, 10U));          /* due 209, then every 50 */
    assert(tsPoll(&ts, 209U, out, 3U) == 1U && tsTimeLeft(&ts, &m1, 2U) == 50U);

    /* increment and decrement, as the generated code writes them: the time
     * left set to more or less of it; used up, the timer expires now */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);              /* due 100 */
    assert(tsSetTimeLeft(&ts, &m1, 1U, tsTimeAdd(tsTimeLeft(&ts, &m1, 1U), 50U)));
    assert(tsTimeLeft(&ts, &m1, 1U) == 150U);
    assert(tsSetTimeLeft(&ts, &m1, 1U, tsTimeSub(tsTimeLeft(&ts, &m1, 1U), 30U)));
    assert(tsTimeLeft(&ts, &m1, 1U) == 120U);
    assert(tsPoll(&ts, 119U, out, 3U) == 0U && tsPoll(&ts, 120U, out, 3U) == 1U);
    assert(!tsSetTimeLeft(&ts, &m1, 1U, tsTimeAdd(tsTimeLeft(&ts, &m1, 1U), 10U)));  /* gone */
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);              /* due 220 */
    assert(tsSetTimeLeft(&ts, &m1, 1U, tsTimeSub(tsTimeLeft(&ts, &m1, 1U), 500U)));  /* more than left */
    assert(tsTimeLeft(&ts, &m1, 1U) == 0U);
    assert(tsPoll(&ts, 120U, out, 3U) == 1U && out[0].machine == &m1);      /* the timeout, now */
    assert(tsRunning(&ts) == 0U);                                             /* and the timer is gone */
    (void)tsStart(&ts, &m1, 3U, 10U, TS_ONCE);
    (void)tsSetTimeLeft(&ts, &m1, 3U, tsTimeAdd(tsTimeLeft(&ts, &m1, 3U), TS_TICKS_MAX));  /* capped */
    assert(tsTimeLeft(&ts, &m1, 3U) == TS_TICKS_MAX);
    assert(tsPoll(&ts, 121U, out, 3U) == 0U);                /* still ahead */
    assert(tsTimeLeft(&ts, &m1, 3U) == TS_TICKS_MAX - 1U);

    /* a TS_EVERY timer keeps its period when its time left changes */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 50U, TS_EVERY);              /* due 50 */
    (void)tsPoll(&ts, 20U, out, 3U);                         /* 30 left */
    assert(tsSetTimeLeft(&ts, &m1, 1U, tsTimeAdd(tsTimeLeft(&ts, &m1, 1U), 100U)));  /* due 150 */
    assert(tsPoll(&ts, 149U, out, 3U) == 0U);
    assert(tsPoll(&ts, 150U, out, 3U) == 1U && tsTimeLeft(&ts, &m1, 1U) == 50U);     /* then every 50 */

    /* a timer may be longer than half the counter's range */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, TS_TICKS_HALF + 10U, TS_ONCE);
    assert(tsPoll(&ts, TS_TICKS_HALF, out, 3U) == 0U && tsTimeLeft(&ts, &m1, 1U) == 10U);
    assert(tsPoll(&ts, TS_TICKS_HALF + 10U, out, 3U) == 1U);

    /* every timers that did not fit into out[] keep their phase */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_EVERY);
    (void)tsStart(&ts, &m2, 1U, 100U, TS_EVERY);
    assert(tsPoll(&ts, 100U, out, 1U) == 1U && out[0].machine == &m1);
    assert(tsTimeLeft(&ts, &m2, 1U) == 0U);                  /* expired, not reported yet */
    assert(tsPoll(&ts, 150U, out, 3U) == 1U && out[0].machine == &m2);
    assert(out[0].periods == 1U);
    assert(tsTimeLeft(&ts, &m2, 1U) == 50U);                 /* next at 200, as m1 */
    assert(tsPoll(&ts, 200U, out, 3U) == 2U);
    /* not reported for lack of room: the count grows until it is */
    assert(tsPoll(&ts, 300U, out, 1U) == 1U && out[0].machine == &m1 && out[0].periods == 1U);
    assert(tsPoll(&ts, 400U, out, 3U) == 2U && out[0].periods == 1U);
    assert(out[1].machine == &m2 && out[1].periods == 2U);   /* 300 not reported, then 400 */
    /* a restart forgets the periods not reported yet */
    assert(tsPoll(&ts, 500U, out, 0U) == 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_EVERY);
    assert(tsPoll(&ts, 600U, out, 3U) == 2U && out[0].machine == &m1 && out[0].periods == 1U);
    assert(out[1].machine == &m2 && out[1].periods == 2U);

    /* an expiry not reported yet is taken back by setting the time left */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 100U, TS_ONCE);
    (void)tsStart(&ts, &m2, 1U, 100U, TS_ONCE);
    assert(tsPoll(&ts, 100U, out, 1U) == 1U && out[0].machine == &m1);
    assert(tsSetTimeLeft(&ts, &m2, 1U, 30U));
    assert(tsPoll(&ts, 100U, out, 3U) == 0U && tsRunning(&ts) == 1U);
    assert(tsPoll(&ts, 130U, out, 3U) == 1U && out[0].machine == &m2);

    /* a time kept outside the service */
    assert(tsTimeAdd(100U, 50U) == 150U && tsTimeSub(100U, 30U) == 70U);
    assert(tsTimeSub(100U, 100U) == 0U && tsTimeSub(100U, 500U) == 0U);
    assert(tsTimeAdd(10U, TS_TICKS_MAX) == TS_TICKS_MAX);                 /* the same cap */

    /* errors go to the handler; a timer that does not run is none */
    tsInit(&ts, table, 3U, 0U);
    errors = 0;
    tsStop(&ts, &m1, 1U);                                    /* not running: no error */
    assert(!tsSetTimeLeft(&ts, &m1, 1U, 10U) && tsTimeLeft(&ts, &m1, 1U) == 0U);
    assert(errors == 0);
    assert(!tsStart(NULL, &m1, 1U, 10U, TS_ONCE) && lastError == TS_ERR_NO_SERVICE);
    assert(!tsStart(&ts, NULL, 1U, 10U, TS_ONCE) && lastError == TS_ERR_NO_MACHINE);
    assert(!tsStart(&ts, &m1, 1U, 0U, TS_EVERY) && lastError == TS_ERR_NO_PERIOD);
    assert(tsTimeLeft(NULL, &m1, 1U) == 0U && lastError == TS_ERR_NO_SERVICE);
    tsStop(&ts, NULL, 1U);
    assert(lastError == TS_ERR_NO_MACHINE);
    assert(tsPoll(&ts, 10U, NULL, 3U) == 0U && lastError == TS_ERR_NO_TABLE);
    tsInit(&ts, NULL, 3U, 0U);
    assert(lastError == TS_ERR_NO_TABLE && ts.capacity == 0U);
    assert(errors == 7);
    errors = 0;

    /* a pending timer: stopped, its report is dropped; restarted, it starts
     * fresh; otherwise it is reported by the next poll */
    tsInit(&ts, table, 3U, 0U);
    (void)tsStart(&ts, &m1, 1U, 10U, TS_ONCE);
    (void)tsStart(&ts, &m2, 1U, 10U, TS_ONCE);
    (void)tsStart(&ts, &m2, 2U, 10U, TS_ONCE);
    assert(tsPoll(&ts, 10U, out, 0U) == 0U);                 /* no room: all three pending */
    assert(tsTimeLeft(&ts, &m1, 1U) == 0U && tsRunning(&ts) == 3U);
    tsStop(&ts, &m2, 1U);                                    /* dropped */
    assert(tsStart(&ts, &m2, 2U, 50U, TS_ONCE));             /* fresh: 50 from 10 */
    assert(tsPoll(&ts, 20U, out, 3U) == 1U && out[0].machine == &m1);       /* the pending report */
    assert(tsPoll(&ts, 59U, out, 3U) == 0U);
    assert(tsPoll(&ts, 60U, out, 3U) == 1U && out[0].timerId == 2U);

    /* a TS_ONCE timer started with 0 ticks expires on the next poll, even at the same time */
    tsInit(&ts, table, 3U, 100U);
    (void)tsStart(&ts, &m1, 1U, 0U, TS_ONCE);
    assert(tsPoll(&ts, 100U, out, 3U) == 1U);

    /* every across the wrap of the clock keeps its phase */
    tsInit(&ts, table, 3U, (TS_TICKS_T)((TS_TICKS_T)0 - 150U));    /* 150 before the wrap */
    (void)tsStart(&ts, &m1, 1U, 100U, TS_EVERY);              /* due at -50, 50, 150, 250 */
    assert(tsPoll(&ts, (TS_TICKS_T)((TS_TICKS_T)0 - 50U), out, 3U) == 1U);
    assert(tsPoll(&ts, 49U, out, 3U) == 0U && tsTimeLeft(&ts, &m1, 1U) == 1U);
    assert(tsPoll(&ts, 175U, out, 3U) == 1U && tsTimeLeft(&ts, &m1, 1U) == 75U);   /* 50 and 150 merged */

    /* the time kept outside the service, at its limits */
    assert(tsTimeAdd(TS_TICKS_MAX, 0U) == TS_TICKS_MAX && tsTimeAdd(0U, TS_TICKS_MAX) == TS_TICKS_MAX);
    assert(tsTimeAdd(1U, TS_TICKS_MAX - 1U) == TS_TICKS_MAX && tsTimeAdd(1U, TS_TICKS_MAX) == TS_TICKS_MAX);
    assert(tsTimeSub(0U, 0U) == 0U && tsTimeSub(TS_TICKS_MAX, TS_TICKS_MAX) == 0U);
    assert(tsTimeSub(TS_TICKS_MAX, 1U) == TS_TICKS_MAX - 1U);

    /* errors without a service, and stopAll without a machine; no table or out[]
     * is fine where nothing would go into it */
    errors = 0;
    tsInit(NULL, table, 3U, 0U);
    assert(lastError == TS_ERR_NO_SERVICE);
    assert(tsPoll(NULL, 0U, out, 3U) == 0U && tsRunning(NULL) == 0U);
    tsStopAll(NULL, &m1);
    tsStopAll(&ts, NULL);
    assert(lastError == TS_ERR_NO_MACHINE && errors == 5);
    tsInit(&ts, NULL, 0U, 0U);
    assert(tsPoll(&ts, 0U, NULL, 0U) == 0U && errors == 5);

    /* durations at run time: the same ticks as the macros, and an error past the range */
    errors = 0;
    assert(tsMs(1500U) == TS_MS(1500U) && tsS(2U) == TS_S(2U) && tsUs(1001U) == TS_US(1001U));
    assert(tsUs(2000U) == TS_US(2000U));                                     /* divides exactly */
    assert(tsMs((uintmax_t)-1) == TS_TICKS_MAX && lastError == TS_ERR_TOO_LONG);   /* e.g. a negative value */
    assert(tsUs((uintmax_t)-1) == TS_TICKS_MAX && tsS((uintmax_t)-1) == TS_TICKS_MAX);
    errors = 0;
#if TS_TICK_US == 1000U
    assert(TS_MS_FITS((uintmax_t)TS_TICKS_MAX) && !TS_MS_FITS((uintmax_t)TS_TICKS_MAX + 1U));
    assert(TS_US_FITS((uintmax_t)TS_TICKS_MAX * 1000U) && !TS_US_FITS((uintmax_t)TS_TICKS_MAX * 1000U + 1U));
    assert(tsMs((uintmax_t)TS_TICKS_MAX) == TS_TICKS_MAX && errors == 0);
    assert(tsMs((uintmax_t)TS_TICKS_MAX + 1U) == TS_TICKS_MAX && errors == 1);
    assert(tsUs((uintmax_t)TS_TICKS_MAX * 1000U) == TS_TICKS_MAX && errors == 1);        /* exactly the limit */
    assert(tsUs((uintmax_t)TS_TICKS_MAX * 1000U + 1U) == TS_TICKS_MAX && errors == 2);   /* rounds up past it */
    assert(tsUs((uintmax_t)TS_TICKS_MAX * 1000U - 999U) == TS_TICKS_MAX && errors == 2); /* rounds up to it */
    assert(TS_US(70000U) == 70U);                            /* divided before it is converted */
    if (sizeof(TS_TICKS_T) == 2U) {                          /* 16-bit ticks of 1 ms: at most 65.535 s */
        assert(TS_S_FITS(65U) && !TS_S_FITS(66U) && TS_S(65U) == 65000U);
        assert(tsS(70U) == TS_TICKS_MAX && lastError == TS_ERR_TOO_LONG);
    }
#endif

    /* durations to ticks */
#if TS_TICK_US == 1000U
    assert(TS_MS(200U) == 200U && TS_S(2U) == 2000U);
    assert(TS_US(1U) == 1U && TS_US(1000U) == 1U && TS_US(1001U) == 2U);   /* rounded up */
    assert(TS_TO_MS(TS_MS(1500U)) == 1500U);
#elif TS_TICK_US == 1U
    assert(TS_US(7U) == 7U && TS_MS(200U) == 200000U && TS_S(2U) == 2000000U);
    assert(TS_TO_MS(TS_MS(1500U)) == 1500U && TS_TO_MS(1999U) == 1U);   /* rounded down */
#endif

    /* the count of periods stops at 65535: in one poll, and summed over polls without room */
    if (TS_TICKS_MAX > 200000U) {
        tsInit(&ts, table, 3U, 0U);
        (void)tsStart(&ts, &m1, 1U, 1U, TS_EVERY);
        assert(tsPoll(&ts, 70000U, out, 3U) == 1U && out[0].periods == 0xFFFFU);
        assert(tsPoll(&ts, 70010U, out, 0U) == 0U);              /* 10, kept */
        assert(tsPoll(&ts, 140010U, out, 0U) == 0U);             /* 10 + 65535 or more */
        assert(tsPoll(&ts, 140010U, out, 3U) == 1U && out[0].periods == 0xFFFFU);
    }

    printf("timer_service: all checks passed (%u-bit time, %u us ticks)\n",
           (unsigned)(sizeof(TS_TICKS_T) * 8U), (unsigned)TS_TICK_US);
    return 0;
}
