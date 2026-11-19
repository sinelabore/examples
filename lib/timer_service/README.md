# Timer service for SinelaboreRT state machines

The code a state machine generated with time events (`after(...)`, `every(...)`, named timers;
`UseTimerService=yes`) calls to start and stop its timers. MIT licence, see [LICENSE](LICENSE):
compile it into your product as it is, or replace it with your own.

| Folder | Service | Timeouts arrive |
| --- | --- | --- |
| `.` | the reference service, C99 (`timer_service.h`, `.c`) and C++11 (`timer_service.hpp`); no heap, no threads, no interrupts | by `tsPoll()` / `poll()`, called from your loop |
| `freertos/` | C, on FreeRTOS software timers | as task notification bits of the machine's task |
| `trampoline/` | C, on AUTOSAR OS / OSEK alarms ([Trampoline](https://github.com/TrampolineRTOS/trampoline)) | as events of the machine's task |
| `freertos_events/` | as `freertos/`, for timeouts as events (`TimeoutsAsEvents=yes`) | through a function of yours, once per expiry and period: post the timer's event into the machine task's queue. On overrun, once per missed period, in a row |
| `trampoline_events/` | as `trampoline/`, for timeouts as events | each alarm activates a task of yours that calls `tsExpired()`, which calls your function as above. On overrun, a missed period is dropped |

Each folder has its own `timer_service.h` with the same names and meanings, so the generated code
compiles unchanged against any of them. Choose one with the include path and compile that
folder's `.c` file:

```make
TIMER_SERVICE ?= ../../lib/timer_service

# reference service, C
CFLAGS += -I$(TIMER_SERVICE)
SRCS   += $(TIMER_SERVICE)/timer_service.c

# or FreeRTOS
CFLAGS += -I$(TIMER_SERVICE)/freertos
SRCS   += $(TIMER_SERVICE)/freertos/freertos_timer_service.c

# or FreeRTOS, timeouts as events
CFLAGS += -I$(TIMER_SERVICE)/freertos_events
SRCS   += $(TIMER_SERVICE)/freertos_events/freertos_events_timer_service.c
```

In event mode the reference service needs nothing extra: your loop looks up each timeout's event
with `<machine>TimeoutEvent()` and sends it once. `periods > 1` means your loop missed periods of
that timer (an overrun); check it if you want to. The `_events` services call a function
of yours instead (`TS_TIMEOUT_FN`), from the timer task or a task the alarm activates; it posts the
event into the queue the machine task waits on, beside the events of keys, buses and the like.

C++ needs only `-I$(TIMER_SERVICE)` and `timer_service.hpp`.

## What the generated code calls

`m` is the machine; in C++ the machine is given by its timers, `sm::MachineTimers t`, a member
of the machine.

| C | C++ | |
| --- | --- | --- |
| `tsStart(ts, m, id, ticks, mode)` | `start(t, id, ticks, mode)` | start or restart a timer, once or every `ticks` |
| `tsStop(ts, m, id)` | `stop(t, id)` | stop it; a timeout not yet taken is dropped |
| `tsStopAll(ts, m)` | `t.stopAll()` | stop every timer of the machine |
| `tsTimeLeft(ts, m, id)` | `timeLeft(t, id)` | ticks until the timer expires, 0 if it does not run |
| `tsSetTimeLeft(ts, m, id, ticks)` | `setTimeLeft(t, id, ticks)` | let a running timer expire `ticks` from now; an every timer keeps its period |
| `tsPoll(ts, now, out, max)` | `poll(now, out, max)` | reference service only, called by your loop: the timers that expired since the last poll |

The C++ functions without `t.` are those of `sm::ITimerService`, the interface a C++ service
implements; `sm::TimerService<N>` is the reference one.

`increment`, `decrement`, `set` and `stop` of a named timer in the model are written over these
calls by the generator. Duration macros (`TS_MS(500)`, `TS_S(3)`) turn times into ticks,
rounded up so a timer never fires early.

## Writing your own

In C, copy the header of the folder closest to your timers and implement `tsStart`, `tsStop`,
`tsStopAll`, `tsTimeLeft` and `tsSetTimeLeft`; in C++, derive from `sm::ITimerService`.
What an implementation has to keep:

- A stopped or restarted timer must not deliver a timeout it set before. Clear its pending
  timeout (bit, event, queue entry) in `stop` and `start`.
- In flag mode, `setTimeLeft` on a once timer that has expired but whose timeout is not taken
  yet starts it again. In event mode the timeout has been posted and stays with the machine;
  `setTimeLeft` answers false.
- Every call comes from the context that runs the machine; only the timer's own callback or
  interrupt may run elsewhere, and it only signals the machine's task.

## Checks

    make test        the reference service in C (32-, 64- and 16-bit ticks) and C++ (C++11, C++17)
    make coverage    line and branch coverage of both, 100% with clang and GCC
    make doc         the API documentation (Doxygen) of every service, in build/doc;
                     fails on any public function, parameter or return value left undocumented

The headers are documented with Doxygen comments (`@param`, `@return`, `@retval`), which IDEs
show as tooltips and clang checks with `-Wdocumentation`.

The RTOS versions are checked by running the microwave examples on them.
