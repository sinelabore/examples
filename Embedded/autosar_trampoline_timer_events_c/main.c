/*
 * Microwave oven on Trampoline (AUTOSAR OS / OSEK) with timeouts as events:
 * the model of ../autosar_trampoline_timer_c with TimeoutsAsEvents, its
 * timers on OS alarms (lib/timer_service/trampoline_events).
 *
 *   buttons (ISR2)       --- evInc, evDoorOpen, evDoorClosed --+
 *                                                               +-->  queue  -->  ovenTask ---> oven (prints)
 *   alarm -> timer task  --- cookTimerTimeout, ... ------------+                     |
 *      ^                                                                            |
 *      +--- SetRelAlarm, CancelAlarm, GetAlarm (the timer service) ------------------+
 *
 * - ovenTask owns the machine. It waits for evQueue and dispatches every
 *   event in the queue in the order it arrived: no polling cycle, and no
 *   steps without event. A timeout is an event like a button:
 *   cookTimerTimeout and Cooking_0Timeout (oven_ext.h).
 * - Each timer's alarm activates a task of its own (microwave.oil), which
 *   calls tsExpired() with its timer id; the service calls onTimeout(),
 *   which looks up the timer's event (ovenTimeoutEvent) and posts it.
 * - The buttons are the POSIX target's virtual board: key a raises the ISR
 *   buttonA, key b the ISR buttonB, q ends the program. The ISRs post the
 *   machine's events.
 *
 * OSEK has no message queue, so the queue is a ring buffer here: posting
 * and taking suspend the OS interrupts for the copy, and a post sets
 * evQueue. ISRs, tasks and the service may post; only ovenTask takes.
 *
 *   make run    a: one second more, b: open or close the door, q: quit
 */
#include <stdbool.h>
#include <stdint.h>
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include "timer_service.h"
#include "tpl_os.h"

#include <stdio.h>

DeclareTask(ovenTask);
DeclareTask(cookTimerTask);
DeclareTask(cookingTickTask);
DeclareEvent(evQueue);
DeclareAlarm(alarmCookTimer);
DeclareAlarm(alarmCookingTick);

/* Events the queue holds. Buttons come one by one and ovenTask takes each
 * at once, timeouts at most one per timer and period: a few entries do. */
#define QUEUE_LENGTH 16U

static OVEN_INSTANCEDATA_T oven_inst = OVEN_INSTANCEDATA_INIT;

/* The timer service of the machine: an alarm per timer id. */
static TS_ALARM_T alarms[OVEN_TM_COUNT];
static TS_SERVICE_T timers;

/* ---- The queue of ovenTask ---- */

static OVEN_MSG_T queue[QUEUE_LENGTH];
static uint16_t queueHead;                  /* the oldest entry */
static uint16_t queueCount;                 /* the entries in it */

/* Post an event to ovenTask, from an ISR or a task. Does not wait: a full
 * queue loses the event, which is reported. */
static void post(const OVEN_MSG_T *msg)
{
    bool posted = false;
    SuspendOSInterrupts();
    if (queueCount < QUEUE_LENGTH) {
        queue[(queueHead + queueCount) % QUEUE_LENGTH] = *msg;
        queueCount++;
        posted = true;
    }
    ResumeOSInterrupts();
    if (posted) {
        (void)SetEvent(ovenTask, evQueue);
    } else {
        printf("queue full: event %u is lost\r\n", (unsigned)msg->msg);
    }
}

/* The oldest event, for ovenTask; false when the queue is empty. */
static bool take(OVEN_MSG_T *msg)
{
    bool taken = false;
    SuspendOSInterrupts();
    if (queueCount > 0U) {
        *msg = queue[queueHead];
        queueHead = (uint16_t)((queueHead + 1U) % QUEUE_LENGTH);
        queueCount--;
        taken = true;
    }
    ResumeOSInterrupts();
    return taken;
}

/* ---- The timeouts ---- */

/* The timer service's function for a timeout (TS_TIMEOUT_FN): post the
 * timer's event. Runs in the timer's task, or in ovenTask for a timer
 * started with 0 ticks. */
static void onTimeout(void *context, TS_TIMER_ID_T timerId)
{
    OVEN_MSG_T msg;
    (void)context;
    msg.msg = ovenTimeoutEvent(timerId);
    post(&msg);
}

/* Activated by alarmCookTimer, once per expiry */
TASK(cookTimerTask)
{
    tsExpired(&timers, OVEN_TM_COOKTIMER);
    (void)TerminateTask();
}

/* Activated by alarmCookingTick, once per period */
TASK(cookingTickTask)
{
    tsExpired(&timers, OVEN_TM_COOKING_0);
    (void)TerminateTask();
}

/* ---- The machine ---- */

int main(void)
{
    printf("a: one second more; b: open or close the door; q: quit\r\n");
    fflush(stdout);                         /* before the virtual board forks */
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

static void dispatch(const OVEN_MSG_T *msg)
{
    (void)oven(&oven_inst, msg);
}

TASK(ovenTask)
{
    OVEN_MSG_T msg;

    /* which alarm belongs to which timer id of the machine, and where its
     * timeouts go; the alarms run only once the machine starts them */
    alarms[OVEN_TM_COOKTIMER].alarm = alarmCookTimer;
    alarms[OVEN_TM_COOKING_0].alarm = alarmCookingTick;
    timers.timeout = onTimeout;
    timers.context = NULL;
    timers.alarms = alarms;
    timers.count = OVEN_TM_COUNT;
    oven_inst.timerService = &timers;

    msg.msg = OVEN_NO_MSG;
    dispatch(&msg);                         /* the first call enters Super and Idle */
    fflush(stdout);

    for (;;) {
        (void)WaitEvent(evQueue);           /* blocked until an event is posted */
        (void)ClearEvent(evQueue);          /* before taking: a post meanwhile sets it again */
        while (take(&msg)) {
            dispatch(&msg);
        }
        fflush(stdout);
    }
}

/* ---- The buttons ---- */

/* Key a */
ISR(buttonA)
{
    OVEN_MSG_T msg;
    msg.msg = evInc;
    post(&msg);
}

/* Key b; the door starts open */
ISR(buttonB)
{
    static bool doorOpen = true;
    OVEN_MSG_T msg;
    doorOpen = !doorOpen;
    msg.msg = doorOpen ? evDoorOpen : evDoorClosed;
    post(&msg);
}
