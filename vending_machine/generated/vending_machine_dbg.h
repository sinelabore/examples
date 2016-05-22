/* (C) Your company*//* Command line options: -l cx -v -p ASTAH -o vending_machine -t machine:vending_machine:ui ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually */
/* Generated on: Sun May 22 11:37:53 CEST 2016 / version 3.6.14 */




#ifndef __VENDING_MACHINE_DBG_H__
#define __VENDING_MACHINE_DBG_H__

/* State names */
const char vending_machine_states[]=
	"WaitForSelection\0PricePayed\0SelectionDone\0FinalState0\0__VENDING_MACHINE_LAST_STATE__\0";


/* State values */
const unsigned short vending_machine_state_values[]={
	WaitForSelection,PricePayed,SelectionDone,FinalState0,__VENDING_MACHINE_LAST_STATE__
};


const unsigned short vending_machine_state_idx[]={
	0,17,28,42,54,85};

#define VENDING_MACHINE_EVENT_START 0


/* Event names */
const char vending_machine_events[]=
	"evReleaseDone\0evProductA\0evCoin\0evUnknownCoin\0evProductC\0evClearSelection\0evProductB\0NO_MSG\0";


/* Event values */
const unsigned short vending_machine_event_values[]={
	evReleaseDone,evProductA,evCoin,evUnknownCoin,evProductC,evClearSelection,evProductB,VENDING_MACHINE_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short vending_machine_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<7;i++){
		if(vending_machine_event_values[i]==evt) return i;
	}
	return 7; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short vending_machine_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<4;i++){
		if(vending_machine_state_values[i]==state) return i;
	}
	return 4; // should never be reached
}



const unsigned short vending_machine_evt_idx[]={
	0,14,25,32,46,57,74,85
};

const char* vending_machineGetNameByState(unsigned short state){
	unsigned short idx = vending_machine_map_state_enum(state);
	return vending_machine_states+vending_machine_state_idx[idx];
}

const char* vending_machineGetNameByEvent(VENDING_MACHINE_EVENT_T evt){
	unsigned short idx = vending_machine_map_event_enum(evt);
	return vending_machine_events+vending_machine_evt_idx[idx];
}


#endif
