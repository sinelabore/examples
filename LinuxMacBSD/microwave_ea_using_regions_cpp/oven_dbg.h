/* (c) Peter Mueller, 2012 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -d -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually*/
/* Generated on: Thu Nov 01 13:52:14 CET 2012 / version 3.0beta2 */


#ifndef __OVEN_DBG_H__
#define __OVEN_DBG_H__

/* State names */
const char oven_states[]=
	"Inactive\0Active\0LightOff\0LightOn\0LowPower\0HighPower\0RadiatorOff\0CookingPause\0Cooking\0__OVEN_LAST_STATE__\0";


/* State values */
const unsigned short oven_state_values[]={
	Inactive,Active,LightOff,LightOn,LowPower,HighPower,RadiatorOff,CookingPause,Cooking,__OVEN_LAST_STATE__
};


const unsigned short oven_state_idx[]={
	0,9,16,25,33,42,52,64,77,85,105};

#define OVEN_EVENT_START 0


/* Event names */
const char oven_events[]=
	"evDoorOpen\0evTimeout\0evDec\0evDoorClosed\0evPwrLow\0evInc\0evPwrHigh\0NO_MSG\0";


/* Event values */
const unsigned short oven_event_values[]={
	evDoorOpen,evTimeout,evDec,evDoorClosed,evPwrLow,evInc,evPwrHigh,OVEN_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven_map_event_enum(unsigned short evt){
	unsigned int i;
	for(i=0; i<7;i++){
		if(oven_event_values[i]==evt) return i;
	}
	return 7; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven_map_state_enum(unsigned short state){
	unsigned int i;
	for(i=0; i<9;i++){
		if(oven_state_values[i]==state) return i;
	}
	return 9; // should never be reached
}



const unsigned short oven_evt_idx[]={
	0,11,21,27,40,49,55,65
};

const char* ovenGetNameByState(unsigned short state){
	unsigned short idx = oven_map_state_enum(state);
	return oven_states+oven_state_idx[idx];
}

const char* ovenGetNameByEvent(OVEN_EVENT_T evt){
	unsigned short idx = oven_map_event_enum(evt);
	return oven_events+oven_evt_idx[idx];
}


#endif
