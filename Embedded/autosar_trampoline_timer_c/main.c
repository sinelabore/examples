/*
 * Microwave oven on Trampoline (AUTOSAR OS / OSEK): the generated C machine of
 * LinuxMacBSD/microwave_builtin_editor_timer_c, with its timers on OS alarms
 * (lib/timer_service/trampoline).
 *
 *   buttons (ISR2) --- evButtonA, evButtonB --->  ovenTask  ---> oven (prints)
 *   alarms         --- timer events ---------->     |
 *      ^                                            |
 *      +--- SetRelAlarm, CancelAlarm, GetAlarm (the timer service)
 *
 * - ovenTask owns the machine. It waits for its events and wakes for a
 *   button or a timeout: no polling cycle. Each timer of the machine has an
 *   alarm that sets an event of its own (microwave.oil); a timeout is handed
 *   to the machine and taken in steps without event, then a button.
 * - The buttons are the POSIX target's virtual board: key a raises the ISR
 *   buttonA, key b the ISR buttonB, q ends the program. An ISR only sets an
 *   event; ovenTask turns it into the machine's event.
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
DeclareEvent(evButtonA);
DeclareEvent(evButtonB);
DeclareEvent(evCookTimer);
DeclareEvent(evCookingTick);
DeclareAlarm(alarmCookTimer);
DeclareAlarm(alarmCookingTick);

static OVEN_INSTANCEDATA_T oven_inst = OVEN_INSTANCEDATA_INIT;

/* The timer service of the machine: an alarm and event per timer id. */
static TS_ALARM_T alarms[OVEN_TM_COUNT];
static TS_SERVICE_T timers;

int main(void)
{
    printf("a: one second more; b: open or close the door; q: quit\r\n");
    fflush(stdout);                         /* before the virtual board forks */
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

/* Hand an event to the machine. */
static void dispatch(OVEN_EVENT_T event)
{
    OVEN_MSG_T msg;
    msg.msg = event;
    (void)oven(&oven_inst, &msg);
}

TASK(ovenTask)
{
    const EventMaskType all = evButtonA | evButtonB | evCookTimer | evCookingTick;
    bool doorOpen = true;
    EventMaskType set;
    uint16_t id;
    uint16_t i;

    /* which alarm and event belong to which timer id of the machine */
    alarms[OVEN_TM_COOKTIMER].alarm = alarmCookTimer;
    alarms[OVEN_TM_COOKTIMER].event = evCookTimer;
    alarms[OVEN_TM_COOKING_0].alarm = alarmCookingTick;
    alarms[OVEN_TM_COOKING_0].event = evCookingTick;
    timers.task = ovenTask;
    timers.alarms = alarms;
    timers.count = OVEN_TM_COUNT;
    oven_inst.timerService = &timers;

    dispatch(OVEN_NO_MSG);                  /* the first call enters Super and Idle */
    fflush(stdout);

    for (;;) {
        (void)WaitEvent(all);
        (void)GetEvent(ovenTask, &set);
        (void)ClearEvent(set & all);

        /* 1. Timeouts: hand each one to the machine, then let it take them in
         *    steps without event; bounded, so conditions that keep winning
         *    cannot hang the task. */
        for (id = 0U; id < OVEN_TM_COUNT; id++) {
            if ((set & alarms[id].event) != 0U) {
                ovenSetTimeout(&oven_inst, id);
            }
        }
        for (i = 0U; (i < OVEN_TM_COUNT) && (ovenTimeoutPending(&oven_inst) != 0U); i++) {
            dispatch(OVEN_NO_MSG);
        }

        /* 2. The buttons */
        if ((set & evButtonA) != 0U) {
            dispatch(evInc);
        }
        if ((set & evButtonB) != 0U) {
            doorOpen = !doorOpen;
            dispatch(doorOpen ? evDoorOpen : evDoorClosed);
        }
        fflush(stdout);
    }
}

/* Key a */
ISR(buttonA)
{
    (void)SetEvent(ovenTask, evButtonA);
}

/* Key b */
ISR(buttonB)
{
    (void)SetEvent(ovenTask, evButtonB);
}
