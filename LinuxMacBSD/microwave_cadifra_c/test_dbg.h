/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -c -p CADIFRA -o test first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually */
/* Generated on: Mon Jan 06 19:13:28 CET 2014 / version 3.5 */




#ifndef __TEST_DBG_H__
#define __TEST_DBG_H__

/* State names */
const char test_states[]=
	"Super\0Completed\0CookingPause\0Idle\0Error\0Cooking\0__TEST_LAST_STATE__\0";


/* State values */
const unsigned short test_state_values[]={
	Super,Completed,CookingPause,Idle,Error,Cooking,__TEST_LAST_STATE__
};


const unsigned short test_state_idx[]={
	0,6,16,29,34,40,48,68};

#define TEST_EVENT_START 0


/* Event names */
const char test_events[]=
	"evDoorOpen\0evDec\0evTimeout\0evPwr\0evDoorClosed\0evInc\0NO_MSG\0";


/* Event values */
const unsigned short test_event_values[]={
	evDoorOpen,evDec,evTimeout,evPwr,evDoorClosed,evInc,TEST_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short test_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<6;i++){
		if(test_event_values[i]==evt) return i;
	}
	return 6; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short test_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<6;i++){
		if(test_state_values[i]==state) return i;
	}
	return 6; // should never be reached
}



const unsigned short test_evt_idx[]={
	0,11,17,27,33,46,52
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
