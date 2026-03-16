/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

#include "system.h"
#include <iostream>

// ========== main ========== //
int main() {
    // Create your custom system
    double n=2;
    Test::A a;

    a.init();

    Test::A::runDef run;   // execution
    run.n = 0;             // set input
    run(a);                // execute

    std::cout << "run.p: " << run.p << " run.u: " << run.u << std::endl;

    return 0;
}
