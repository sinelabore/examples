Microwave oven on AUTOSAR OS, with the timers of its state machine on OS
alarms.

The model and the generated C code are those of
../../LinuxMacBSD/microwave_builtin_editor_timer_c, unchanged. Only the timer
service differs: ../../lib/timer_service/trampoline implements the same
functions on AUTOSAR OS / OSEK alarms, so the generated code compiles against
it as it is.

The OS is Trampoline (https://github.com/TrampolineRTOS/trampoline), an
open-source AUTOSAR OS / OSEK implementation, on its POSIX target, so it
builds and runs on macOS and Linux without a board.

- Each timer of the machine has an alarm and an event, declared in
  microwave.oil. The alarm sets the event in ovenTask.
- ovenTask waits for its events (WaitEvent). It hands each timeout to the
  machine (ovenSetTimeout), lets the machine take them in steps without
  event, then handles a button.
- The POSIX target's virtual board owns the keyboard: key a and key b arrive
  as interrupts (ISR2). a adds a second, b opens or closes the door; the door
  starts open.
- One tick is 10 ms; ViPER, a process of Trampoline, sends it.

  export CODEGEN_PATH=<folder of codegen.jar>
  make            fetches Trampoline and builds goil and ViPER on first use
                  (a few minutes), then builds ./oven
  make run        starts it: a, a, b cooks two seconds; q quits

./oven needs ViPER and finds it through VIPER_PATH, which make run sets.
To start it yourself:

  VIPER_PATH=$PWD/trampoline/viper ./oven

Tested on macOS (Apple Silicon). Needs Python 3 and a C++ compiler for goil.
Needs a SinelaboreRT version with time events (UseTimerService).
