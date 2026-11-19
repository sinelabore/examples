Microwave oven with time events, in C++, model created with the built-in
state diagram editor.

The model of ../microwave_builtin_editor_timer_c, generated as C++ (-l cppx),
with the reference timer service of ../../lib/timer_service
(timer_service.hpp). The manual explains time events in "Time-Triggered
Transitions"; the readme of the C example explains the model.

- The machine holds its timers as a member (sm::MachineTimers) and gets the
  service as a constructor parameter (sm::ITimerService&); main.cpp passes
  an sm::TimerService<N>.
- main.cpp runs the machine in a loop: poll the service, hand each timeout to
  its machine (setTimeout), let it take them in steps without event, then
  dispatch a key.

  export CODEGEN_PATH=<folder of codegen.jar>
  make            generate and build (C++11)
  ./oven          run it: + - o c p P q
  ./edit.sh       open the model in the editor

Needs a SinelaboreRT version with time events (UseTimerService).
