/* (C) Your company *//* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Tue Oct 27 19:53:47 CET 2026 / 8.0 */




#ifndef OVEN_EXT_H 
#define OVEN_EXT_H 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evDec=0U,
	evDoorClosed,
	evDoorOpen,
	evInc,
	evPwr,
	evTimeout,
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
