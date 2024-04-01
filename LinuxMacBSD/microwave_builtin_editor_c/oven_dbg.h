/* (C) Your company *//* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually */
/* Generated on: Fri Mar 15 11:23:55 CET 2024 / Version 6.3.2.3814 */




#ifndef OVEN_DBG_H
#define OVEN_DBG_H

/* State names */
const char oven_states[]=
	"Super\0Completed\0Cooking\0CookingPause\0Idle\0__OVEN_LAST_STATE__\0";


/* State values */
const unsigned short oven_state_values[]={
	Super,Completed,Cooking,CookingPause,Idle,__OVEN_LAST_STATE__
};


#define InnermostStates "Super;"
const unsigned short oven_state_idx[]={
	0,6,16,24,37,42,62};

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
