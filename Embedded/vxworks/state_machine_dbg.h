/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -E -p ssc   */
/* This file is generated from state_machine.xml - do not edit manually */
/* Generated on: Tue Mar 07 15:02:52 CET 2023 / Version 5.5.6.3515 */




#ifndef STATE_MACHINE_DBG_H
#define STATE_MACHINE_DBG_H

/* State names */
const char state_machine_states[]=
	"Fast\0Final0\0RUN\0STOP\0Slow\0__STATE_MACHINE_LAST_STATE__\0";


/* State values */
const unsigned short state_machine_state_values[]={
	Fast,Final0,RUN,STOP,Slow,__STATE_MACHINE_LAST_STATE__
};


#define InnermostStates "Fast,Final0,STOP,Slow;"
const unsigned short state_machine_state_idx[]={
	0,5,12,16,21,26,55};

#define STATE_MACHINE_EVENT_START 0


/* Event names */
const char state_machine_events[]=
	"evShutdown\0evTimeout\0evFast\0evStart\0evStop\0evSlow\0NO_MSG\0";


/* Event values */
const unsigned short state_machine_event_values[]={
	evShutdown,evTimeout,evFast,evStart,evStop,evSlow,STATE_MACHINE_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short state_machine_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<6;i++){
		if(state_machine_event_values[i]==evt) return i;
	}
	return 6; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short state_machine_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<5;i++){
		if(state_machine_state_values[i]==state) return i;
	}
	return 5; // should never be reached
}



const unsigned short state_machine_evt_idx[]={
	0,11,21,28,36,43,50
};

const char* state_machineGetNameByState(unsigned short state){
	unsigned short idx = state_machine_map_state_enum(state);
	return state_machine_states+state_machine_state_idx[idx];
}

const char* state_machineGetNameByEvent(STATE_MACHINE_EVENT_T evt){
	unsigned short idx = state_machine_map_event_enum((unsigned short)evt);
	return state_machine_events+state_machine_evt_idx[idx];
}


#endif
