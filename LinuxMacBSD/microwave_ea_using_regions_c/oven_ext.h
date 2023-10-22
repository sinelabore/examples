/* (C) Your company*//* Command line options: -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Oct 22 18:01:32 CEST 2023 / Version 6.1.3706 */




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
