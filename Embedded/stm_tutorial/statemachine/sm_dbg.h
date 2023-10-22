/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options:   */
/* This file is generated from sm.xml - do not edit manually */
/* Generated on: Sun Oct 08 21:03:24 CEST 2023 / Version 6.1.3705 */




#ifndef SM_DBG_H
#define SM_DBG_H

/* State names */
const char sm_states[]=
	"Failsafe\0Off\0On\0Operational\0__SM_LAST_STATE__\0";


/* State values */
const unsigned short sm_state_values[]={
	Failsafe,Off,On,Operational,__SM_LAST_STATE__
};


#define InnermostStates "Failsafe,Off,On;"
const unsigned short sm_state_idx[]={
	0,9,13,16,28,46};

#define SM_EVENT_START 0


/* Event names */
const char sm_events[]=
	"evTimeout\0evGoFailsafe\0NO_MSG\0";


/* Event values */
const unsigned short sm_event_values[]={
	evTimeout,evGoFailsafe,SM_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short sm_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<2;i++){
		if(sm_event_values[i]==evt) return i;
	}
	return 2; /* should never be reached */
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short sm_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<4;i++){
		if(sm_state_values[i]==state) return i;
	}
	return 4; /* should never be reached */
}



const unsigned short sm_evt_idx[]={
	0,10,23
};

const char* smGetNameByState(unsigned short state){
	unsigned short idx = sm_map_state_enum(state);
	return sm_states+sm_state_idx[idx];
}

const char* smGetNameByEvent(SM_EVENT_T evt){
	unsigned short idx = sm_map_event_enum((unsigned short)evt);
	return sm_events+sm_evt_idx[idx];
}


#endif
