/* (C) Your company *//* Command line options: -p ssc -l cx -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Tue Nov 17 19:39:10 CET 2026 / 8.0 */




#ifndef OVEN_EXT_H 
#define OVEN_EXT_H 

/*Events which can be sent to the state-machine  */
typedef enum
{
	Cooking_0Timeout=0U,
	cookTimerTimeout,
	evDec,
	evDoorClosed,
	evDoorOpen,
	evInc,
	evPwr,
	OVEN_NO_MSG
} OVEN_EVENT_T;

/* Message carrying the event and, for events that have fields, their payload. */
typedef struct {
	uint16_t pwr;
} OVEN_EVPWR_T;

typedef struct {
	OVEN_EVENT_T msg;
	union {
		OVEN_EVPWR_T evPwr;
	} data;
} OVEN_MSG_T;


#endif
