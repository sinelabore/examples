/* (C) Your company*//* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Oct 22 17:46:34 CEST 2023 / Version 6.1.3706 */




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
