/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p ssc sm.xml   */
/* This file is generated from sm.xml - do not edit manually */
/* Generated on: Fri Jul 19 18:23:30 CEST 2024 / Version 6.3.3. */




#ifndef SM_DBG_H
#define SM_DBG_H

/* State names */
const char sm_states[]=
	"FAST\0ON\0SLOW\0__SM_LAST_STATE__\0";


/* State values */
const unsigned short sm_state_values[]={
	FAST,ON,SLOW,__SM_LAST_STATE__
};


#define InnermostStates "FAST,ON,SLOW;"
const unsigned short sm_state_idx[]={
	0,5,8,13,31};

#define SM_EVENT_START 0


/* Event names */
const char sm_events[]=
	"evtTimeout\0NO_MSG\0";


/* Event values */
const unsigned short sm_event_values[]={
	evtTimeout,SM_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short sm_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<1;i++){
		if(sm_event_values[i]==evt) return i;
	}
	return 1; /* should never be reached */
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short sm_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<3;i++){
		if(sm_state_values[i]==state) return i;
	}
	return 3; /* should never be reached */
}



const unsigned short sm_evt_idx[]={
	0,11
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
