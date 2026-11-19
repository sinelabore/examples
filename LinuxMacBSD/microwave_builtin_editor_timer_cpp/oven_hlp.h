#ifndef OVEN_HLP_H
#define OVEN_HLP_H

#include <cstdint>
#include "timer_service.hpp"

//
// The oven's hardware. The cooking time and its timer are in the
// state machine (oven.xml).
//

// Shows the cooking time left on the display, in ticks of the timer service
// as TIME_LEFT(cookTimer) gives them
void ovenDisplay(sm::Ticks left);

// Sets power, in watts (evPwr carries it)
void ovenSetPwr(std::uint16_t watts);

// Switch off microwave generator
void ovenOff();

// Switch on microwave generator with preset
void ovenOn();

#endif
