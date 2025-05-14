/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -Trace -p CADIFRA -o complex complex.cdd   */
/* This file is generated from complex.cdd - do not edit manually */
/* Generated on: Wed May 14 21:43:07 CEST 2025 / 6.5.3 */




#ifndef COMPLEX_DBG_H
#define COMPLEX_DBG_H

/* State names */
const char complex_states[]=
	"S2\0S21\0S22\0S1\0S11\0S12\0S3\0S4\0__COMPLEX_LAST_STATE__\0";


/* State values */
const unsigned short complex_state_values[]={
	S2,S21,S22,S1,S11,S12,S3,S4,__COMPLEX_LAST_STATE__
};


#define InnermostStates "S21,S22,S11,S12,S3,S4;"
const unsigned short complex_state_idx[]={
	0,3,7,11,14,18,22,25,28,51};

#define COMPLEX_EVENT_START 0


/* Event names */
const char complex_events[]=
	"e20\0e11\0e21\0e10\0e13\0e12\0e15\0e14\0e16\0e1\0e2\0e4\0e5\0e6\0e7\0e8\0e9\0NO_MSG\0";


/* Event values */
const unsigned short complex_event_values[]={
	e20,e11,e21,e10,e13,e12,e15,e14,e16,e1,e2,e4,e5,e6,e7,e8,e9,COMPLEX_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short complex_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<17;i++){
		if(complex_event_values[i]==evt) return i;
	}
	return 17; /* should never be reached */
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short complex_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<8;i++){
		if(complex_state_values[i]==state) return i;
	}
	return 8; /* should never be reached */
}



const unsigned short complex_evt_idx[]={
	0,4,8,12,16,20,24,28,32,36,39,42,45,48,51,54,57,60
};

const char* complexGetNameByState(unsigned short state){
	unsigned short idx = complex_map_state_enum(state);
	return complex_states+complex_state_idx[idx];
}

const char* complexGetNameByEvent(COMPLEX_EVENT_T evt){
	unsigned short idx = complex_map_event_enum((unsigned short)evt);
	return complex_events+complex_evt_idx[idx];
}


#endif
