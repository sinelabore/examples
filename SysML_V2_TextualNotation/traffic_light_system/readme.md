# Traffic Light Control System

This example implements a simple system to manage traffic lights in a city.
It consists of a central control center called `TrafficManagementCenter` and decentralized traffic light controllers called `TrafficLightController`.

Each of the two parts contains a state machine and uses control ports for interconnection.
A system parts weaves the basic parts together.

In this simple example, there is just one traffic light that is by default in the `OutOfService` state and then set to `Operational` by the central controller.


## Note:
- In `main.cpp`, derived classes of the state machines are implemented to provide required methods needed to run the demo. Nested parts must be initialized, and event handlers must be implemented to forward events to the state machine engine.
- The main routine regularly calls the `process()` method to run the simulation
- The Makefile and project were tested on Darwin.