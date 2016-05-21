

#ifndef __DISPLAY_WITH_COINBANK___
#define __DISPLAY_WITH_COINBANK___

#include "lib/fifo.h"

void display_init(FIFO_T* const fifo);

void displayValue(uint8_t val);

uint8_t getCoinValue(void);
void releaseCoins(uint8_t centsToRelease);

#endif
