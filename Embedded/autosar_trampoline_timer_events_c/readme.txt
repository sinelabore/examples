Microwave oven on AUTOSAR OS with timeouts as events: one queue for buttons
and timeouts, and a machine task that waits on it.

This is ../autosar_trampoline_timer_c with "Timeouts as events" switched on
in the model (<ssc:TimeoutsAsEvents>yes</ssc:TimeoutsAsEvents>; in the
editor: "Edit state machine code"). The machine is the same; only how a
timeout reaches it differs:

- The generator gives each timer an event, cookTimerTimeout and
  Cooking_0Timeout (oven_ext.h), and ovenTimeoutEvent(), which maps a timer
  id to its event. ovenSetTimeout() and ovenTimeoutPending() are not
  generated.
- The timer service is ../../lib/timer_service/trampoline_events. Each timer
  has an alarm that activates a small task of its own (microwave.oil), once
  per expiry, an every timer once per period. The task calls tsExpired()
  with its timer id; the service calls onTimeout() in main.c, which looks up
  the event and posts it. (An alarm that set an event would merge periods,
  and an alarm callback may not wake a task, hence the task.)
- The buttons' ISRs post their events into the same queue.
- ovenTask waits for one event, evQueue, and dispatches every queued event
  in the order it arrived. There is no polling and no step without event.

OSEK has no message queue, so main.c has a small ring buffer: posting and
taking suspend the OS interrupts for the copy, and a post sets evQueue.

The OS is Trampoline (https://github.com/TrampolineRTOS/trampoline), an
open-source AUTOSAR OS / OSEK implementation, on its POSIX target, so it
builds and runs on macOS and Linux without a board. The POSIX target's
virtual board owns the keyboard: key a and key b arrive as interrupts
(ISR2). a adds a second, b opens or closes the door; the door starts open.
One tick is 10 ms; ViPER, a process of Trampoline, sends it.

What to keep in mind: a timeout waits in the queue until ovenTask takes it.
A timer stopped meanwhile is no problem, the machine ignores its event; a
timer restarted meanwhile would take the old event early, so ovenTask
should take its queue promptly. The flag version,
../autosar_trampoline_timer_c, has no such case.

Overrun: the system has to keep up with its fastest timer. The timer tasks
are declared with ACTIVATION = 1 and run above ovenTask. If a timer's task
cannot run before its alarm expires again, the OS refuses that activation
(E_OS_LIMIT, reported to the ErrorHook if one is configured) and that
period's timeout is dropped; the service does not see it. A larger
ACTIVATION would deliver the missed periods one after the other instead.

  export CODEGEN_PATH=<folder of codegen.jar>
  make            fetches Trampoline and builds goil and ViPER on first use
                  (a few minutes), then builds ./oven
  make run        starts it: a, a, b cooks two seconds; q quits

./oven needs ViPER and finds it through VIPER_PATH, which make run sets.
To start it yourself:

  VIPER_PATH=$PWD/trampoline/viper ./oven

Tested on macOS (Apple Silicon). Needs Python 3 and a C++ compiler for goil.
Needs a SinelaboreRT version with timeouts as events (TimeoutsAsEvents).
