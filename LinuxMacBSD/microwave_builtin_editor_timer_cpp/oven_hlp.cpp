#include "oven_hlp.h"
#include <cstdio>

void ovenDisplay(sm::Ticks left)
{
	const std::uint32_t ms = sm::toMs(left);
	std::printf("Oven::display %lu.%lus\n", static_cast<unsigned long>(ms / 1000U),
	            static_cast<unsigned long>((ms % 1000U) / 100U));
}

void ovenSetPwr(std::uint16_t watts)
{
	std::printf("Oven::set power to %u W\n", static_cast<unsigned>(watts));
}

void ovenOff()
{
	std::printf("Oven::off\n");
}

void ovenOn()
{
	std::printf("Oven::on\n");
}
