/* The POSIX target's virtual board puts the terminal into raw mode, so
 * lines end with \r\n. */
#include "oven_hlp.h"
#include <stdio.h>

void ovenDisplay(TS_TICKS_T left)
{
    const uint32_t ms = (uint32_t)TS_TO_MS(left);
    printf("Oven::display %lu.%lus\r\n", (unsigned long)(ms / 1000U), (unsigned long)((ms % 1000U) / 100U));
}

void ovenSetPwr(uint16_t watts)
{
    printf("Oven::set power to %u W\r\n", (unsigned)watts);
}

void ovenOff(void)
{
    printf("Oven::off\r\n");
}

void ovenOn(void)
{
    printf("Oven::on\r\n");
}
