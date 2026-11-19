// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Peter Mueller
// Compile check: a global reference service must be constant-initialized,
// so that a machine in another file can start timers in its constructor
// (CallInitializeInCtor). constinit needs C++20; the header itself needs
// only C++17.
#include "timer_service.hpp"

constinit sm::TimerService<4U> service;

int main()
{
    return 0;
}
