Microwave oven on FreeRTOS, with the timers of its state machine on FreeRTOS
software timers.

The model and the generated C code are those of
../../LinuxMacBSD/microwave_builtin_editor_timer_c, unchanged. Only the timer
service differs: ../../lib/timer_service/freertos implements the same
functions on FreeRTOS, so the generated code compiles against it as it is.

It runs on FreeRTOS's POSIX port, so it builds and runs on macOS and Linux
without a board.

- Each timer of the machine has a FreeRTOS software timer. Its callback sets
  a notification bit of the machine task, bit = timer id.
- The machine task waits for its bits (xTaskNotifyWait): no polling. It hands
  each timeout to the machine (ovenSetTimeout), lets the machine take them in
  steps without event, then dispatches a key.
- A keyboard task looks for a key every 20 ms and signals it with bit 31.
- The timer task has the highest priority (FreeRTOSConfig.h): the service
  relies on a timer command being carried out before the call returns.

  export CODEGEN_PATH=<folder of codegen.jar>
  make            fetches the FreeRTOS kernel (V11.3.3) on first use, builds ./oven
  ./oven          keys: + - o c p P q

The POSIX port's tick runs slow on macOS (about 790 ticks per second at
1000 Hz), so a second of cooking takes about 1.25 s of wall time.

Needs a SinelaboreRT version with time events (UseTimerService).
