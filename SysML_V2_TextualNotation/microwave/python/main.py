"""Drives the Oven by hand — the Python twin of ../cpp/main.cpp.

  + / -   one wheel tick up / down   -> Wheel.doInc / Wheel.doDec
  c / o   close / open the door      -> DoorContact.contact
  d       debug trace on / off
  q       quit

Nothing here carries a value from one part to another - the model's own 'connect'
statements do that, in Oven.init(). This file only schedules and watches.

Reading an output: a Port keeps no value, it hands what is sent to whoever is connected
and forgets it. add_receiver() adds one more listener beside the ones connect() made, so
watching a port here does not disturb the wiring. Each cycle starts by clearing what was
seen, so the table shows what was sent in *this* cycle and a '.' where nothing was.

One cycle is one tick of the countdown, not one second.
"""
from __future__ import annotations

import os
import select
import sys
import termios
import time
import tty

import framework
from system import Oven


class RawTerminal:
    """Terminal in raw mode while this object lives, so keys arrive unbuffered.

    Falls back to doing nothing when stdin is not a terminal, which is what happens
    when the driver is fed from a pipe; select() below then reports the pipe instead.
    """

    def __enter__(self) -> "RawTerminal":
        self.saved = None
        if sys.stdin.isatty():
            self.saved = termios.tcgetattr(sys.stdin)
            tty.setcbreak(sys.stdin.fileno())
        return self

    def __exit__(self, *exc: object) -> None:
        if self.saved is not None:
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, self.saved)


def keys_waiting() -> str:
    """Whatever has been typed since the last look, without waiting for more."""
    typed = ""
    while select.select([sys.stdin], [], [], 0)[0]:
        char = os.read(sys.stdin.fileno(), 1).decode(errors="ignore")
        if not char:
            break
        typed += char
    return typed


def name_of(value: object) -> str:
    """A command or a wheel tick as the table prints it; '.' for nothing sent."""
    if value is None or getattr(value, "name", "None") == "None":
        return "."
    return value.name


def state_of(display: object) -> str:
    machine = display.stateMachine
    if machine.isInZERO():
        return "ZERO"
    if machine.isInPAUSE():
        return "PAUSE"
    if machine.isInCOUNTDOWN():
        return "COUNTDOWN"
    return "-"


def main() -> None:
    oven = Oven()
    oven.instance_name = "oven"
    oven.init()

    # system.py is generated with -d, so every action and accept reports itself. Off
    # until 'd' asks for it, or the trace buries the table.
    debug_sink = framework.trace_sink
    framework.trace_sink = None

    seen: dict[str, object] = {"door": None, "time": None, "wheel": None,
                               "lamp": None, "motor": None, "magnetron": None}

    def watch(key: str, field_name: str):
        """Follows what a port sends into seen[key], beside whatever it is connected to."""
        def receive(sent) -> None:
            seen[key] = getattr(sent.cmd, field_name)
        return receive

    # The three appliances are levels, not events: the Controller commands one only when
    # the timer tells it to, and a lamp stays lit until told otherwise - so those three
    # keep the last command rather than being cleared each cycle (see the loop).
    oven.doorContact.doorSignal.add_receiver(watch("door", "cmd"))
    oven.timerDisplay.timePort.add_receiver(watch("time", "cmd"))
    oven.wheel.wheel.add_receiver(watch("wheel", "tick"))
    oven.controller.cmdPortToLamp.add_receiver(watch("lamp", "cmd"))
    oven.controller.cmdPortToMotor.add_receiver(watch("motor", "cmd"))
    oven.controller.cmdPortToMagnetron.add_receiver(watch("magnetron", "cmd"))

    with RawTerminal():
        print("+/- wheel   c/o door   d trace   q quit\r")
        print("Nominal Power of Device: %g Watt\r" % oven.nominalPower)
        print("cycle | wheel.wheel  doorSignal  timePort | ticks  state     "
              "|  lamp  motor  magnetron\r")

        cycle = 0
        while True:
            seen["door"] = seen["time"] = seen["wheel"] = None

            for key in keys_waiting():
                if key == "+":
                    oven.wheel.doInc()
                elif key == "-":
                    oven.wheel.doDec()
                elif key == "c":
                    oven.doorContact.contact = True
                elif key == "o":
                    oven.doorContact.contact = False
                elif key == "d":
                    framework.trace_sink = (
                        None if framework.trace_sink is not None else debug_sink)
                elif key == "q":
                    return

            # One scan. process() walks the containment tree and steps each part's state
            # machine, and each machine runs its states' 'do' actions - DoorContact
            # publishes the contact, TimerDisplay reads the wheel and counts down.
            oven.process()

            print("%5d | %11s  %10s  %8s | %5d  %-10s| %5s%7s%11s\r"
                  % (cycle, name_of(seen["wheel"]), name_of(seen["door"]),
                     name_of(seen["time"]), oven.timerDisplay.adjustedTicks,
                     state_of(oven.timerDisplay), name_of(seen["lamp"]),
                     name_of(seen["motor"]), name_of(seen["magnetron"])),
                  flush=True)

            cycle += 1
            time.sleep(0.2)


if __name__ == "__main__":
    main()
