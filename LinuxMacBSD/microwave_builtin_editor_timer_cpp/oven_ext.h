/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p ssc -l cppx -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sat Nov 14 15:02:19 CET 2026 / 8.0 */


#ifndef OVEN_EXT_H
#define OVEN_EXT_H

// Events which can be sent to the state-machine
	enum OVEN_EVENT_T {
			evDec=0U,
			evDoorClosed,
			evDoorOpen,
			evInc,
			evPwr,
			OVEN_NO_MSG
	};

// Message carrying the event and, for events that have fields, their payload.
struct OVEN_EVPWR_T {
	uint16_t pwr;
};

struct OVEN_MSG_T {
	OVEN_EVENT_T id;
	union {
		OVEN_EVPWR_T evPwr;
	} data;
};






#endif
