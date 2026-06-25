/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include "system.h"

using namespace TrafficLight;


// --- Main ---
int main() {
    TrafficLightSystem tls;

    //setup subcomponents
    tls.init();

    // init state machine
    tls.tmc->initialize();
    tls.tlc->initialize();
    
    // Start process thread
    for ( int i = 0; i < 80; i++ ) {

        tls.tmc->tmcStateMachine();
        tls.tlc->tlcStateMachine();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "[Main] Done.\n";
    return 0;
}
