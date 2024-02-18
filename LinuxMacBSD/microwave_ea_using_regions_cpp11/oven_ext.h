/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cppx -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Feb 18 13:09:47 CET 2024 / Version 6.3.2.3812 */


#ifndef OVEN_EXT_H
#define OVEN_EXT_H

#include <cstdint>

// Events which can be sent to the state-machine
	enum class OVEN_EVENT_T : std::uint16_t {
			evPwrLow=0U,
			evTimeout,
			evDec,
			evDoorOpen,
			evDoorClosed,
			evPwrHigh,
			evInc,
			OVEN_NO_MSG
	};






#endif
