# Examples
Example projects showing how to generate code from state machines using the Sinelabore code generator. 

SinelaboreRT was built especially for embedded software developers. It focuses on just one task: Generation of readable and maintainable code from hierarchical UML state machines and from activity diagrams. With its unique features the tool covers well the requirements of embedded real-time and low power application developers.

Examples:
- EnergiaBlink: Realzing a blinker using a state machine. Showing how to trace the event flow using Mscgen (http://www.sinelabore.com/doku.php?id=wiki:toolbox:trace-the-event-flow)
- Vending Machine: Example for a more complex example of using state machines. Here some state machines work together to realize the system requirements.
- sm_using_timer_queue_osx_freebsd_linux: Example that shows how to use state machines with timers and queues. The state machine receives its events from an event queue. The queue is filled from timer events, other state machines (cooperating machines) or interrupt handlers. You can compile this example on Linux/OS-X and FreeBSD.
- sm_using_timer_queue_win32: Example that shows how to use state machines with timers and queues. The state machine receives its events from an event queue. The queue is filled from timer events, other state machines (cooperating machines) or interrupt handlers. To compile this example on Windows install the MinGW compiler or use Visual Studio. 
See more on www.sinelabore.com
