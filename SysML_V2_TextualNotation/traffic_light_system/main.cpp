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

class MyTrafficLightController : public TrafficLightController {
 public:

    void init(void) override{
        test1 = std::make_unique<Test>();
        initialize();
    }

    void onTimeout(const TLCEvent& e) override{
        std::cout << "[MyTrafficLightController] Timeout fired with id="
            << getNameByEvent(e)<< std::endl;
        recvPort.receive(ControlPortDataDef{e});
        tlcStateMachine();
    }

    void handle(const ControlPortDataDef& e) override{
        std::cout << "[MyTrafficLightController] received: value = "
            << getNameByEvent(e.data.msg) << "\n";
        tlcStateMachine();
    }
};

class MyTrafficManagementCenter : public TrafficManagementCenter {
 public:

    void init(void) override{
        test1 = std::make_unique<Test>();
        initialize();
    }

    void onTimeout(const TMCEvent& event) override{
        std::cout << "[MyTrafficManagementCenter] Timeout fired with id=" << getNameByEvent(event) << std::endl;
        msg = event;
        tmcStateMachine();
    }
};

class MyTrafficLightSystem : public TrafficLightSystem{
 public:
    void init(void) {
        tmc = std::make_unique<MyTrafficManagementCenter>();
        tlc = std::make_unique<MyTrafficLightController>();
        TrafficLightSystem::init();
    }
};


// --- Main ---
int main() {
    MyTrafficLightSystem tls;

    tls.init();

    // Start process thread
    for ( int i = 0; i < 80; i++ ) {
        tls.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    std::cout << "[Main] Done.\n";
    return 0;
}
