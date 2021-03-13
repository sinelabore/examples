/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p ssc -o test test.xml   */
/* This file is generated from test.xml - do not edit manually */
/* Generated on: Sat Mar 13 15:08:00 CET 2021 / Version 5.0.6b1 */




#ifndef __TEST_DBG_H__
#define __TEST_DBG_H__

/* State names */
const char test_states[]=
	"Active\0Inactive\0Off\0On\0__TEST_LAST_STATE__\0";


/* State values */
const unsigned short test_state_values[]={
	Active,Inactive,Off,On,__TEST_LAST_STATE__
};


#define InnermostStates "Inactive,Off,On;"
const unsigned short test_state_idx[]={
	0,7,16,20,23,43};

#define TEST_EVENT_START 0


/* Event names */
const char test_events[]=
	"evTimeoutA\0evTimeoutB\0NO_MSG\0";


/* Event values */
const unsigned short test_event_values[]={
	evTimeoutA,evTimeoutB,TEST_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short test_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<2;i++){
		if(test_event_values[i]==evt) return i;
	}
	return 2; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short test_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<4;i++){
		if(test_state_values[i]==state) return i;
	}
	return 4; // should never be reached
}



const unsigned short test_evt_idx[]={
	0,11,22
};

const char* testGetNameByState(unsigned short state){
	unsigned short idx = test_map_state_enum(state);
	return test_states+test_state_idx[idx];
}

const char* testGetNameByEvent(TEST_EVENT_T evt){
	unsigned short idx = test_map_event_enum(evt);
	return test_events+test_evt_idx[idx];
}


#endif
