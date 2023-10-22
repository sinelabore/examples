/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p UModel -l cx -t MicrowaveOvenWithRegions:MachineClass -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually  */
/* Generated on: Sun Oct 22 18:28:05 CEST 2023 / Version 6.1.3706 */




#ifndef OVEN_EXT_H 
#define OVEN_EXT_H 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evPwrLow=0U,
	evTimeout,
	evDec,
	evDoorOpen,
	evDoorClosed,
	evPwrHigh,
	evInc,
	OVEN_NO_MSG
} OVEN_EVENT_T;


#endif
