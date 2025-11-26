#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include "TrafficLightControllerSM.h"
#include "TrafficManagementCenterSM.h"


class MyTrafficLightController : public TrafficLightControllerSM {
    public:

    void init(void) {
        test1 = std::make_unique<Test>();
        TrafficLightController::init();
        initialize();
    }

    virtual void onTimeout(const TRAFFICLIGHTCONTROLLERSM_EVENT_T& event){
        std::cout << "[MyTrafficLightController] Timeout fired with id=" 
            << getNameByEvent(event)<< std::endl;
        processEvent(event);
    }

    virtual void handle(const ControlPortDataDef& e) {
        std::cout << "  [MyTrafficLightController] received: value = "  
            << getNameByEvent(e.msg) << "," << e.test << "\n";
        processEvent(e.msg);
    }
};

class MyTrafficManagementCenter : public TrafficManagementCenterSM {
    public:

    void init(void) {
        test1 = std::make_unique<Test>();
        TrafficManagementCenterSM::init();
        initialize();
    }

    void onTimeout(const TRAFFICMANAGEMENTCENTERSM_EVENT_T& event) {
        std::cout << "[MyTrafficManagementCenter] Timeout fired with id=" << getNameByEvent(event) << std::endl;
        processEvent(event);
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
    for(int i = 0; i < 80; i++) {
        tls.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    std::cout << "[Main] Done.\n";
    return 0;
}
