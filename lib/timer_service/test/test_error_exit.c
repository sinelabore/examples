/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Peter Mueller
 */
/* The default error handler ends the program: `make test` expects exit code 1. */
#include "timer_service.h"

static int machine;

int main(void)
{
    (void)tsStart(NULL, &machine, 1U, 10U, TS_ONCE);    /* no service: tsErrorExit() */
    return 0;                                           /* not reached */
}
