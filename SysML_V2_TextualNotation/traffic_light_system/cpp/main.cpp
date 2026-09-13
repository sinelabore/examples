/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/*
 * Runs the traffic light system - the C++ twin of ../python/main.py.
 *
 * Nothing here drives a part by name. init() builds the sub-parts and wires every
 * 'connect' of the model; process() is one scan of the whole system: it steps the state
 * machines a part drives and then the parts it contains. Everything you see at run time
 * therefore comes from the model.
 */
#include <chrono>
#include <iostream>
#include <thread>

#include "system.h"

using namespace TrafficLight;

int main() {
    TrafficLightSystem tls;

    tls.init();

    // The sleep is the simulation tick. Keep it well below the shortest timeout in the
    // model - 0.5 s here - or a timed transition is stepped over.
    for (int i = 0; i < 80; i++) {
        tls.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "[Main] Done.\n";
    return 0;
}
