Microwave oven with time events, model created with the built-in state
diagram editor.

This is ../microwave_builtin_editor_c with the hand-written timer replaced by
time events and the reference timer service (../../lib/timer_service). The
manual explains time events in "Time-Triggered Transitions".

- cookTimer is a named timer: it belongs to the machine and keeps its time
  while it does not run.
- + and - change that time in every state: Super's internal transitions
  write cookTimer: increment(1s) and cookTimer: decrement(1s), and show
  TIME_LEFT(cookTimer). While Cooking runs they change the running timer;
  - down to 0 ends the cooking.
- Cooking starts the timer with the time it holds: cookTimer: after().
  Leaving Cooking keeps the time left, so opening the door pauses it, and
  closing it cooks on. Idle starts only with a time: [TIME_LEFT(cookTimer) > 0U].
- The generated code uses the id OVEN_TM_COOKTIMER; oven.h also defines the
  name, cookTimer, for calls of the timer service in the application.
- "Timer service" is ticked in "Edit state machine code"; the model file
  stores it as <ssc:UseTimerService>yes</ssc:UseTimerService>.
- main.c runs the machine in a loop of 100 ms: poll the timer service, hand
  each timeout to the machine, let it take them in steps without event, then
  dispatch a key.

  export CODEGEN_PATH=<folder of codegen.jar>
  make            generate and build
  ./oven          run it: + - o c p P q
  ./edit.sh       open the model in the editor

Needs a SinelaboreRT version with time events (UseTimerService).
