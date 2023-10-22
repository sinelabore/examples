/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cx -p md -t Model:oven_package:oven -o oven oven.xmi.xml   */
/* This file is generated from oven.xmi.xml - do not edit manually */
/* Generated on: Sun Oct 22 18:11:21 CEST 2023 / Version 6.1.3706 */




#ifndef OVEN_DBG_H
#define OVEN_DBG_H

/* State names */
const char oven_states[]=
	"Active\0Idle\0Cooking\0CookingPause\0Completed\0__OVEN_LAST_STATE__\0";


/* State values */
const unsigned short oven_state_values[]={
	Active,Idle,Cooking,CookingPause,Completed,__OVEN_LAST_STATE__
};


#define InnermostStates "Idle,Cooking,CookingPause,Completed;"
const unsigned short oven_state_idx[]={
	0,7,12,20,33,43,63};

#define OVEN_EVENT_START 0


/* Event names */
const char oven_events[]=
	"evTimeout\0evDec\0evDoorOpen\0evDoorClosed\0evInc\0evPwr\0NO_MSG\0";


/* Event values */
const unsigned short oven_event_values[]={
	evTimeout,evDec,evDoorOpen,evDoorClosed,evInc,evPwr,OVEN_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<6;i++){
		if(oven_event_values[i]==evt) return i;
	}
	return 6; /* should never be reached */
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<5;i++){
		if(oven_state_values[i]==state) return i;
	}
	return 5; /* should never be reached */
}



const unsigned short oven_evt_idx[]={
	0,10,16,27,40,46,52
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
