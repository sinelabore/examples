// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Peter Mueller
// The default error handler ends the program: `make test` expects exit code 1.
#include "timer_service.hpp"

int main()
{
    sm::TimerService<1U> service(0U);
    (void)service.poll(10U, nullptr, 1U);       // no out[]: sm::timerErrorExit()
    return 0;                                   // not reached
}
