/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p UModel -l cx -t MicrowaveOvenWithRegions:MachineClass -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually */
/* Generated on: Sun Oct 22 18:28:05 CEST 2023 / Version 6.1.3706 */




#ifndef OVEN_DBG_H
#define OVEN_DBG_H

/* State names */
const char oven_states[]=
	"Active\0Inactive\0LightOn\0LightOff\0HighPower\0LowPower\0RadiatorOff\0CookingPause\0Cooking\0__OVEN_LAST_STATE__\0";


/* State values */
const unsigned short oven_state_values[]={
	Active,Inactive,LightOn,LightOff,HighPower,LowPower,RadiatorOff,CookingPause,Cooking,__OVEN_LAST_STATE__
};


#define InnermostStates "Active,Inactive;"
const unsigned short oven_state_idx[]={
	0,7,16,24,33,43,52,64,77,85,105};

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
