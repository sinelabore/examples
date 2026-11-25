/*
 * Microwave oven on embOS-Ultra, on an STM32F405 emulated by QEMU, with
 * timeouts as events: the model of ../freertos_posix_timer_events_c, its
 * timers on embOS software timers (lib/timer_service/embos_ultra_events).
 *
 *   USART1 receive interrupt --- key event -------+
 *                                                 +-->  mailbox  -->  machine task ---> oven
 *   embOS software timer ------- timeout event ---+                       |          (printf on
 *      ^                                                                  |           USART1)
 *      +--- start, stop, time left (the timer service) -------------------+
 *
 * - The machine task owns the machine. It blocks on its mailbox and
 *   dispatches each event in the order it arrived. A timeout is an event
 *   like a key: cookTimerTimeout and Cooking_0Timeout (oven_ext.h).
 * - A key arrives in the console's receive interrupt (board/uart.c), which
 *   calls onKey(): it turns the key into its event and posts it.
 * - The timer service calls onTimeout() for each expiry, an every timer
 *   once per period, in the timer's callback: it looks up the timer's event
 *   (ovenTimeoutEvent) and posts it.
 * Both post with OS_MAILBOX_Put(), which never waits, as interrupts and
 * timer callbacks must not.
 *
 * Keys: + - o c p P q
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "RTOS.h"
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include "timer_service.h"
#include "uart.h"

/* power levels the 'p' and 'P' keys send with evPwr, in watts */
#define PWR_MEDIUM_W 400U
#define PWR_HIGH_W 1200U

/* Keys come one by one and the machine takes each at once, timeouts at
 * most one per timer and period: a few entries do. */
#define MAILBOX_LENGTH 16U

static OVEN_INSTANCEDATA_T ovenInst = OVEN_INSTANCEDATA_INIT;

/* The timer service of the machine: an embOS software timer per timer id. */
static TS_TIMER_T timerList[OVEN_TM_COUNT];
static TS_SERVICE_T timers;

/* The machine task's mailbox: every event of the machine, keys and timeouts. */
static OS_MAILBOX mailbox;
static OVEN_MSG_T mailboxBuffer[MAILBOX_LENGTH];

static OS_STACKPTR int machineStack[512];
static OS_TASK machineTask;

/* ---- The keys: in the console's receive interrupt ---- */

static void onKey(uint8_t c)
{
    OVEN_MSG_T m;
    m.msg = OVEN_NO_MSG;
    if (c == '+') {
        m.msg = evInc;
    } else if (c == '-') {
        m.msg = evDec;
    } else if (c == 'c') {
        m.msg = evDoorClosed;
    } else if (c == 'o') {
        m.msg = evDoorOpen;
    } else if (c == 'P') {
        m.msg = evPwr;
        m.data.evPwr.pwr = PWR_HIGH_W;
    } else if (c == 'p') {
         m.msg = evPwr;
        m.data.evPwr.pwr = PWR_MEDIUM_W;
    } else if ((c == '\r') || (c == '\n')) {
        return;                         /* a line end, e.g. from a test script */
    } else {
       return; // no valid key
    }
    (void)OS_MAILBOX_Put(&mailbox, &m); /* a full mailbox drops the key */
}

/* ---- The timeouts: in the timer's callback ---- */

/* The timer service's function for a timeout (TS_TIMEOUT_FN): post the
 * timer's event. It must not wait: a full mailbox drops the timeout. */
static void onTimeout(void *context, TS_TIMER_ID_T timerId)
{
    OVEN_MSG_T m;
    m.msg = ovenTimeoutEvent(timerId);
    (void)OS_MAILBOX_Put((OS_MAILBOX *)context, &m);
}

/* ---- The machine ---- */

static void machine(void)
{
    OVEN_MSG_T m = {OVEN_NO_MSG,PWR_MEDIUM_W};
    (void)oven(&ovenInst, &m);     /* the first call enters Super and Idle */
    uartConsoleInit(onKey);        /* from now on keys come in */

    for (;;) {
        OS_MAILBOX_GetBlocked(&mailbox, &m);    /* blocked until an event arrives */
        (void)oven(&ovenInst, &m);
    }
}

int main(void)
{
    OS_Init();
    OS_InitHW();
    setvbuf(stdout, NULL, _IONBF, 0);   /* each printf goes out at once, without a heap buffer */

    OS_MAILBOX_Create(&mailbox, sizeof(OVEN_MSG_T), MAILBOX_LENGTH, mailboxBuffer);
    tsInit(&timers, onTimeout, &mailbox, timerList, OVEN_TM_COUNT);
    ovenInst.timerService = &timers;

    OS_TASK_CREATE(&machineTask, "Machine", 100, machine, machineStack);
    OS_Start();                         /* does not return */
    return EXIT_FAILURE;
}
