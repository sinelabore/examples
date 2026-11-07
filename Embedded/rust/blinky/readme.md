This example blinks an LED with a state machine generated as `no_std` Rust. It runs on a
NUCLEO-L432KC board (STM32L432KC, Cortex-M4) and, without a board, in QEMU.

`Operational` blinks the LED slowly (500 ms). Pulling pin A0 (PA0) to GND sends `evGoFailsafe`:
`Failsafe` blinks fast (100 ms) until the pin is released and `evResume` returns to
`Operational`. The machine shows composite states, regions, entry code and timers.

The state machine code in `src/fsm/blinky.rs` is generated from the model `model/blinky.xml`. It
uses `core` only: no `std`, no allocation, no `unsafe`. It is checked in, so the example builds
without the code generator.

* `model/blinky.xml`: The state diagram, created with the built-in editor (`make edit`).
* `codegen.cfg`: Names the two types every generated handler takes: `RustDataType` (the board,
  passed as `data`) and `RustTimerServiceType` (the timers, passed as `timer_service`). The
  actions in the model call their methods, e.g. `data.led_on();`.
* `src/fsm/blinky.rs`: The state machine, generated from `model/blinky.xml`.
* `src/ticker.rs`: Ten software timer slots. Actions start and stop a slot by id; the main loop
  polls them and forwards the event a slot stored.
* `src/lib.rs`: The library with the machine, and a host `Board` that sets a flag instead of a
  GPIO pin, so the machine also builds and runs on the PC.
* `nucleo-l432kc/`: The firmware. `Board` switches the user LED LD3 (PB3); the main loop reads
  PA0 and polls the timers every 10 ms.
* `qemu/`: The same machine on QEMU's LM3S6965EVB (Cortex-M3). `Board` prints over semihosting,
  and a schedule stands in for the failsafe wire.

Run it:

    cargo test                                    # host build and tests
    rustup target add thumbv7m-none-eabi
    cd qemu && cargo run                          # needs qemu-system-arm; runs 3 s

    rustup target add thumbv7em-none-eabihf
    cd nucleo-l432kc && cargo run --release       # flashes over the board's ST-Link, needs probe-rs

To change the model and regenerate the code, set `CODEGEN_PATH` to the folder with
`codegen.jar` and call `make edit` or `make`.
