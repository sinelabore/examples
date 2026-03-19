/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */


#include <system.h>
#include <iostream>

// ========== main ========== //
int main() {
    // Create your custom system
    MyPackage::BatterySystem a;

    a.init();

    MyPackage::BatterySystem::ChargeBatteryDef run;   

    a.batLevel=3;
    std::cout << "batLevel: " << a.batLevel << std::endl;
    run(a);                
    std::cout << "batLevel: " << a.m << std::endl;
    std::cout << "-----"<< std::endl;
    a.batLevel=7;
    std::cout << "batLevel: " << a.batLevel << std::endl;
    run(a);                
    std::cout << "batLevel: " << a.m << std::endl;
    std::cout << "-----"<< std::endl;
    a.batLevel=91;
    std::cout << "batLevel: " << a.batLevel << std::endl;
    run(a);                
    std::cout << "batLevel: " << a.m << std::endl;
    std::cout << "-----"<< std::endl;
    a.batLevel=98;
    std::cout << "batLevel: " << a.batLevel << std::endl;
    run(a);                
    std::cout << "batLevel: " << a.m << std::endl;
    std::cout << "-----"<< std::endl;
    return 0;
}
