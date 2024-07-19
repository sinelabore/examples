/*
 * (c) Peter Mueller, 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -t Model:Class Model:decoder -p EA -o decoder decoder.xml   */
/* This file is generated from decoder.xml - do not edit manually */
/* Generated on: Wed Dec 28 17:29:00 CET 2022 / Version 5.5.5.6 */




#ifndef DECODER_DBG_H
#define DECODER_DBG_H

/* State names */
const char decoder_states[]=
	"WaitForMinStart\0Year\0MinDetected\0Minute\0Month\0Day\0IgnoreBBKMeteo\0DayOfWeek\0Hour\0__DECODER_LAST_STATE__\0";


/* State values */
const unsigned short decoder_state_values[]={
	WaitForMinStart,Year,MinDetected,Minute,Month,Day,IgnoreBBKMeteo,DayOfWeek,Hour,__DECODER_LAST_STATE__
};


#define InnermostStates "WaitForMinStart,Year,Minute,Month,Day,IgnoreBBKMeteo,DayOfWeek,Hour;"
const unsigned short decoder_state_idx[]={
	0,16,21,33,40,46,50,65,75,80,103};

#define DECODER_EVENT_START 0


/* Event names */
const char decoder_events[]=
	"evTimingError\0evMinStart\0evTick\0NO_MSG\0";


/* Event values */
const unsigned short decoder_event_values[]={
	evTimingError,evMinStart,evTick,DECODER_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short decoder_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<3;i++){
		if(decoder_event_values[i]==evt) return i;
	}
	return 3; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short decoder_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<9;i++){
		if(decoder_state_values[i]==state) return i;
	}
	return 9; // should never be reached
}



const unsigned short decoder_evt_idx[]={
	0,14,25,32
};

const char* decoderGetNameByState(unsigned short state){
	unsigned short idx = decoder_map_state_enum(state);
	return decoder_states+decoder_state_idx[idx];
}

const char* decoderGetNameByEvent(DECODER_EVENT_T evt){
	unsigned short idx = decoder_map_event_enum(evt);
	return decoder_events+decoder_evt_idx[idx];
}


#endif
