/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p EA -t Model:implementation:oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually */
/* Generated on: Thu Feb 15 13:54:32 CET 2024 / Version 6.3.2.3798 */




#ifndef OVEN_DBG_H
#define OVEN_DBG_H

/* State names */
const char oven_states[]=
	"Inactive\0Active\0LightOn\0LightOff\0HighPower\0LowPower\0CookingPause\0Cooking\0RadiatorOff\0__OVEN_LAST_STATE__\0";


/* State values */
const unsigned short oven_state_values[]={
	Inactive,Active,LightOn,LightOff,HighPower,LowPower,CookingPause,Cooking,RadiatorOff,__OVEN_LAST_STATE__
};


#define InnermostStates "Inactive,Active;"
const unsigned short oven_state_idx[]={
	0,9,16,24,33,43,52,65,73,85,105};

#define OVEN_EVENT_START 0


/* Event names */
const char oven_events[]=
	"evPwrLow\0evTimeout\0evDec\0evDoorOpen\0evDoorClosed\0evPwrHigh\0evInc\0NO_MSG\0";


/* Event values */
const unsigned short oven_event_values[]={
	evPwrLow,evTimeout,evDec,evDoorOpen,evDoorClosed,evPwrHigh,evInc,OVEN_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<7;i++){
		if(oven_event_values[i]==evt) return i;
	}
	return 7; /* should never be reached */
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<9;i++){
		if(oven_state_values[i]==state) return i;
	}
	return 9; /* should never be reached */
}



const unsigned short oven_evt_idx[]={
	0,9,19,25,36,49,59,65
};

const char* ovenGetNameByState(unsigned short state){
	unsigned short idx = oven_map_state_enum(state);
	return oven_states+oven_state_idx[idx];
}

const char* ovenGetNameByEvent(OVEN_EVENT_T evt){
	unsigned short idx = oven_map_event_enum((unsigned short)evt);
	return oven_events+oven_evt_idx[idx];
}


#endif
