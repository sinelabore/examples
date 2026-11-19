Microwave oven on FreeRTOS with timeouts as events: one queue for keys and
timeouts, and a machine task that blocks on it.

This is ../freertos_posix_timer_c with "Timeouts as events" switched on in
the model (<ssc:TimeoutsAsEvents>yes</ssc:TimeoutsAsEvents>; in the editor:
"Edit state machine code"). The machine is the same; only how a timeout
reaches it differs:

- The generator gives each timer an event, cookTimerTimeout and
  Cooking_0Timeout (oven_ext.h), and ovenTimeoutEvent(), which maps a timer
  id to its event. ovenSetTimeout() and ovenTimeoutPending() are not
  generated.
- The timer service is ../../lib/timer_service/freertos_events. Each timer's
  FreeRTOS callback calls onTimeout() in main.c, once per expiry, an every
  timer once per period. onTimeout() runs in the timer task, looks up the
  event and posts it into the machine task's queue without waiting.
- The keyboard task looks for a key every 20 ms and posts its event into the
  same queue.
- The machine task blocks in xQueueReceive() and dispatches each event in
  the order it arrived. There is no polling and no step without event.

What this buys: one channel and one order for all events, and a timeout can
be posted by hand like any other event. What to keep in mind: a timeout
waits in the queue until the machine task takes it. A timer stopped
meanwhile is no problem, the machine ignores its event; a timer restarted
meanwhile would take the old event early, so the machine task should take
its queue promptly. The flag version, ../freertos_posix_timer_c, has no such
case.

Overrun: the system has to keep up with its fastest timer. If the timer
task is held up for longer than a period, FreeRTOS calls an every timer's
callback once for each missed period, in a row, and the service passes each
call on: the queue receives a burst of timeouts, neither merged nor
reported. The application can recognise it if it needs to; the queue
(QUEUE_LENGTH in main.c) has room for a few.

  export CODEGEN_PATH=<folder of codegen.jar>
  make            fetches the FreeRTOS kernel (V11.3.3) on first use, builds ./oven
                  (FREERTOS_KERNEL=<path> uses an existing checkout)
  ./oven          keys: + - o c p P q

The POSIX port's tick runs slow on macOS (about 790 ticks per second at
1000 Hz), so a second of cooking takes about 1.25 s of wall time.

Needs a SinelaboreRT version with timeouts as events (TimeoutsAsEvents).
