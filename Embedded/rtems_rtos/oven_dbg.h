/* (c) Peter Mueller, 2010 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven oven.cdd   */
/* This file is generated from oven.cdd - do not edit manually*/
/* Generated on: Wed Nov 24 21:11:29 CET 2010 / version 2.2 */


#ifndef __OVEN_DBG_H__
#define __OVEN_DBG_H__

/* State names */
const char oven_states[]=
	"Super\0Completed\0CookingPause\0Idle\0Cooking\0";

const unsigned short oven_state_idx[]={
	0,6,16,29,34,42};

#define OVEN_EVENT_START 0


/* Event names */
const char oven_events[]=
	"evDoorOpen\0evDec\0evTimeout\0evPwr\0evDoorClosed\0evInc\0NO_MSG\0";


const unsigned short oven_evt_idx[]={
	0,11,17,27,33,46,52};

const char* ovenGetNameByState(unsigned short state){
	return oven_states+oven_state_idx[state];
}

const char* ovenGetNameByEvent(OVEN_EVENT_T evt){
	return oven_events+oven_evt_idx[evt-OVEN_EVENT_START];
}


#endif
