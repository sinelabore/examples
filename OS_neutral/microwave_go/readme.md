# Oven Example on GO basis

This example simulates a microwave oven using the GO programming language. GO is somehow similar to C, but adds some very nice new concepts. A highlight are go routines with the select and the channel concept. This allows to write very elegant threading code without the need to use pthreads or other more complex methods. Some other interesting features are the availability of maps and slices, generics, the concept of embedding, tickers and timers to name some others. All of these features were used in this oven state machine example to create a simple but still powerful command line simulation which is very easy to extend.

[Oven Model](oven.png)

## Function:

* `main.go`: Initializes everything and then and sends keyboard events to the `reactor`. `termbox-go` is used to capture keyboard strokes in a non-blocking way
* `rector.go`: The state machine is embedded in the reactor struct, which is implemented as an active object. The main working function is a go coroutine using GO's select mechanism. It uses a ticker as system tick. On each tick, the `Tick()` function of the `countdown_timer` is called. If there is a timeout or any keyboard data, the related events are sent to the oven state machine's `ProcessEvent()` function.
* `countdown_counter.go`: Is a reusable module to provide cyclic and one-shot timers to a state machine. `Tick()` must be called on cyclic basis which returns the expired timers. A state machine can register an event (of type `any` be be generic) which is then sent to the state machine once the timer expired.
* `oven.go`: The state machine fully genrated from the state diagram model (oven.xml). The oven uses the `countdown_counter` to schedule timers.
This example uses two timers. One cyclic timer is used to flash an LED if the oven is running. The other one is used for the cooking time set by the user (single-shot).

The `reactor` and `countdown_counter` can be reused for any state machine that requires timer features and interaction with GO's channels.
