This example simulates a microwave oven in Rust. It is the Rust port of
`LinuxMacBSD/microwave_builtin_editor_c`: the same state diagram, with the action code written in
Rust.

The state machine code in `src/oven.rs` is generated from the model `oven.xml`. It uses `core`
only: no `std`, no allocation, no `unsafe`. The rest of the example uses `std` and no other
crates, so it runs unchanged on Windows, macOS and Linux.

* `oven.xml`: The state diagram, created with the built-in editor (`make edit`).
* `codegen.cfg`: Names the two types every generated handler takes: `RustDataType` (the oven
  hardware, passed as `data`) and `RustTimerServiceType` (the cooking timer, passed as
  `timer_service`). The actions in the model call their methods, e.g. `data.on();`.
* `src/oven.rs`: The state machine, generated from `oven.xml` (`make` regenerates it).
* `src/magnetron.rs`: The microwave generator. It prints what the hardware would do.
* `src/cooking_timer.rs`: The cooking time. It counts down against the clock (`Instant`); while it
  runs it has a deadline, while paused the time left.
* `src/main.rs`: Turns keys into events, and waits for the next key no longer than until the
  cooking time is up, which then becomes `evTimeout`. `evPwr` carries the power in watts:
  `OvenEvents::evPwr { pwr: 1200 }`.

Run it:

    cargo run              # keys: + - o c p P q, each line sent with Enter

To change the model and regenerate the code, set `CODEGEN_PATH` to the folder with
`codegen.jar` and call `make edit` or `make`.
