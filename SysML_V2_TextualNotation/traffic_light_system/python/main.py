"""Runs the traffic light system - the Python twin of ../cpp/main.cpp.

Nothing here drives a part by name. init() builds the sub-parts and wires every
'connect' of the model; process() is one scan of the whole system: it steps the state
machines a part drives and then the parts it contains. Everything you see at run time
therefore comes from the model.

framework.py beside this file is a symlink to the runtime that ships with the product,
the same one the C++ target uses as framework.h.
"""
from __future__ import annotations

import time

from system import TrafficLightSystem


def main() -> None:
    tls = TrafficLightSystem()

    tls.init()

    # The sleep is the simulation tick. Keep it well below the shortest timeout in the
    # model - 0.5 s here - or a timed transition is stepped over.
    for _ in range(80):
        tls.process()
        time.sleep(0.1)
    print("[Main] Done.")


if __name__ == "__main__":
    main()
