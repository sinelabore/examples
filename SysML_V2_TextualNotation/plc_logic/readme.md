# PLC Logic — a block library in SysML v2

A library of logic blocks and the circuits you can build from them: gates, timers, flip-flops,
a ripple counter, a shift register, an SR latch and a 4 bit adder. Every block is a SysML v2
part with ports and a small state machine, and the whole thing compiles to one C++ header.

Run `make`, then `./testcase TestCounter4`, and a four bit counter counts.

## What you can learn from it

This example is a good place to learn how **parts, ports and connections** turn into code,
because it uses almost nothing else. Every block is the same shape — a few ports, a few
attributes, one action, one state machine that does nothing but run the action — so what
differs between `BoolAnd2` and a 4 bit adder is only how the parts are wired. That makes the
generated header easy to read against the model.

It covers well:

| Feature | Where to look |
|---|---|
| `part def` with `in` / `out` ports | any block in `LogicBasic.sysml` |
| conjugated ports (`~BoolPort`) | every input port — an input is the conjugate of an output |
| `port def` carrying an attribute payload | `BoolPort` / `BoolSignal` in `LogicBasic.sysml` |
| `connect a.q to b.i` between sibling parts | `74xxx.sysml`, all of it |
| `bind` — delegating a boundary port to a port inside | `Counter4`, `ShiftReg4`, `Decoder2to4` |
| composition: parts made of parts, three deep | `TestCounter4` → `Counter4` → `T_FF` |
| `accept … via port` to receive | `action exec` in every block with an input |
| `send new … via port` to transmit | every block with an output |
| `accept after N [SI::second]` — real-time timers | `TON`, `TOF`, `AsyncBlink` |
| state machines with `do` and `entry` | every block |
| attribute defaults and redefinition (`:>>`) | `TestCountUp`, `TestToff` |
| SVG generation of both views | the `.svg` targets in the `Makefile` |

It does **not** cover: `item def` payloads, specialization between parts (`:>`), `exhibit
state`, enumerations, multiplicity/collections, or the Python back end. For those, read the
[traffic light system](../traffic_light_system/readme.md) next — it is the complementary
example.

### Before you copy this

The framework runs **one `process()` per scan**, and a port is a queue with a single reader. A
block sends into the next block's queue, and that block reads it when *it* runs. So a wire
costs **nothing** if the receiving block is declared after the sender, and **one whole scan**
if it is declared before.

That is why the blocks inside `Counter4` are declared in the order the carry travels.

`ShiftReg4` shows the opposite case. Its four stages are declared **against** the signal,
because all four are clocked together and each must capture what its neighbour held *before*
the edge. Declared the other way, the bit races through all four stages in a single scan and
you get a wire instead of a register.

`SRLatch` is the third case: a cycle, which cannot be declared in signal order at all. There the one scan it costs to go round the loop *is* the storage.

Those three comments in the models are the most useful thing here, and they apply to any
block network you build, not just to these.

## What is in the folder

| File | Contents |
|---|---|
| `LogicBasic.sysml` | what a wire carries (`BoolPort`, `BoolSignal`), the constant and clock sources, the fan-out, and the flip-flops |
| `IEC61131_3.sysml` | the IEC 61131-3 standard blocks: `BoolAnd2/3`, `BoolNot`, `BoolNand2`, `BoolOr2`, `BoolXor2`, `R_TRIG`, `F_TRIG`, `TON`, `TOF`, `CTUD` |
| `74xxx.sysml` | the circuits a 74xx part gives you: `Counter4`, `ShiftReg4`, `Decoder2to4`, `SRLatch`, `FullAdder`, `Adder4` |
| `test.sysml` | one `Test…` part per network — the harnesses, nothing reusable |
| `main.cpp` | the scan loop and the printing. It only schedules and watches; the blocks read each other through the model's own wiring |
| `codegen.cfg` | generator settings |
| `*.svg` | generated drawings, see below |

Ten networks can be run:

    ./testcase TestTOn          constant → and → not → not → on-delay timer
    ./testcase TestToff         a pulse into an off-delay timer
    ./testcase TestFallingEdge  a pulse into rising- and falling-edge detectors
    ./testcase TestCountUp      a blinking clock into an up/down counter that resets itself
    ./testcase TestAsyncBlink   a free-running blinker into a three-input and
    ./testcase TestCounter4     4 bit ripple counter, one count per scan
    ./testcase TestShiftReg4    4 bit shift register as a Johnson counter
    ./testcase TestDecoder2to4  2-to-4 decoder, half a 74HC139
    ./testcase TestSRLatch      SR latch from two cross-coupled NAND gates
    ./testcase TestAdder4       4 bit adder, a running count plus three

The seven that are clocked by the scan print the same table every run. The three driven by
real-time timers (`TestTOn`, `TestCountUp`, `TestAsyncBlink`) do not: how many scans fit in a
tenth of a second is the scheduler's business.

## Building

`CODEGEN_PATH` must point at the folder holding `codegen.jar`:

    export CODEGEN_PATH=/path/to/sinelabore
    make                    # generates system.h, builds ./testcase, draws the diagrams
    make svg                # only the drawings
    make clean

`make` regenerates `system.h` only when a `.sysml` file is newer, so after upgrading the
generator, delete the outputs (`make clean`) to pick up the change.

## The drawings

`-l svg` draws two things, and which one you get depends on what `-t` names:

- a part with parts inside it and no machine of its own draws the **interconnection view** —
  the blocks, their ports, and what connects them. Every drawing checked in here is one of
  these;
- a block that has a state machine draws that machine instead. None of the `Makefile` targets
  asks for one, but any of the blocks will:

      make svg                     # the ten interconnection views
      $JAVA -t IEC61131_3::TON -l svg -p sysml2text -o TON *.sysml

In an interconnection view, inputs sit on the left of a block and outputs on the right, a
delegated port is joined to the port it stands for by a blue line, and a small dot marks where
one wire branches into several. `-pc none` leaves the text out and draws the structure alone —
that is the difference between `TestOne.svg` and `TestTOn.svg`, which are the same part.

## Extending it

**A new block.** Copy the closest one — `BoolAnd2` for combinational logic, `T_FF` for
something with memory — into `LogicBasic.sysml` or `IEC61131_3.sysml`. Keep the shape: ports,
attributes for whatever it has to remember between scans, one `action exec`, one state machine
that does nothing but `do exec`.

**A new circuit.** Put it in `74xxx.sysml` and give it the ports the chip has. Declare the
blocks inside it in the order the signal travels, unless it is a register or a loop — see
above.

**A new test.** Add a `Test…` part to `test.sysml` wiring your circuit to a clock, copy one of
the functions in `main.cpp`, add a line to `main`, and add an `.svg` target to the `Makefile`
if you want the drawing.

**Consider the following** A part's `in` port is a queue with one reader, so it can stand
for exactly one port inside it: a clock that has to reach four blocks needs `BoolFanOut4`, not
four `connect` statements from the boundary port. And a block with several inputs reads them in
order, one event per input per scan, so every input needs a source that sends every scan — all
the blocks here do.
