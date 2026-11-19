#ifndef __STARTER_HLP_H__
#define __STARTER_HLP_H__

#include <stdint.h>
#include "timer_service.h"

//
// The oven's hardware. The cooking time and its timer are in the
// state machine (oven.xml).
//

// Shows the cooking time left on the display, in ticks of the timer service
// as TIME_LEFT(cookTimer) gives them
void ovenDisplay(TS_TICKS_T left);

// Sets power, in watts (evPwr carries it)
void ovenSetPwr(uint16_t watts);

// Switch off microwave generator
void ovenOff(void);

// Switch on microwave generator with preset
void ovenOn(void);

#endif
