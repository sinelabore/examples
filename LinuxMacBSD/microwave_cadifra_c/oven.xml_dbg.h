/* (c) Peter Mueller, 2012 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven.xml first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually */
/* Generated on: Thu Mar 07 18:11:53 CET 2013 / version 3.11 */




#ifndef __OVEN.XML_DBG_H__
#define __OVEN.XML_DBG_H__

/* State names */
const char oven.xml_states[]=
	"Super\0Completed\0CookingPause\0Idle\0Cooking\0__OVEN.XML_LAST_STATE__\0";


/* State values */
const unsigned short oven.xml_state_values[]={
	Super,Completed,CookingPause,Idle,Cooking,__OVEN.XML_LAST_STATE__
};


const unsigned short oven.xml_state_idx[]={
	0,6,16,29,34,42,66};

#define OVEN.XML_EVENT_START 0


/* Event names */
const char oven.xml_events[]=
	"evDoorOpen\0evDec\0evTimeout\0evPwr\0evDoorClosed\0evInc\0NO_MSG\0";


/* Event values */
const unsigned short oven.xml_event_values[]={
	evDoorOpen,evDec,evTimeout,evPwr,evDoorClosed,evInc,OVEN.XML_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven.xml_map_event_enum(unsigned short evt){
	unsigned int i;
	for(i=0; i<6;i++){
		if(oven.xml_event_values[i]==evt) return i;
	}
	return 6; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short oven.xml_map_state_enum(unsigned short state){
	unsigned int i;
	for(i=0; i<5;i++){
		if(oven.xml_state_values[i]==state) return i;
	}
	return 5; // should never be reached
}



const unsigned short oven.xml_evt_idx[]={
	0,11,17,27,33,46,52
};

const char* oven.xmlGetNameByState(unsigned short state){
	unsigned short idx = oven.xml_map_state_enum(state);
	return oven.xml_states+oven.xml_state_idx[idx];
}

const char* oven.xmlGetNameByEvent(OVEN.XML_EVENT_T evt){
	unsigned short idx = oven.xml_map_event_enum(evt);
	return oven.xml_events+oven.xml_evt_idx[idx];
}


#endif
