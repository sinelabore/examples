/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cx -v -p ASTAH -o oven -t final:oven:machine oven_model.asta   */
/* This file is generated from oven_model.asta - do not edit manually  */
/* Generated on: Mon Apr 01 17:59:18 CEST 2024 / Version 6.3.2.3814 */




#ifndef OVEN_EXT_H 
#define OVEN_EXT_H 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evTimeout=0U,
	evDec,
	evDoorOpen,
	evDoorClosed,
	evInc,
	evPwr,
	OVEN_NO_MSG
} OVEN_EVENT_T;


#endif
