/* (C) Your company*//* Command line options: -l cx -v -p ASTAH -o product_store_sm -t machine:product_store_sm:sm ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually */
/* Generated on: Sun May 22 11:37:57 CEST 2016 / version 3.6.14 */




#ifndef __PRODUCT_STORE_SM_DBG_H__
#define __PRODUCT_STORE_SM_DBG_H__

/* State names */
const char product_store_sm_states[]=
	"Idle\0MotorRunning\0ReleasingOutletDoor\0__PRODUCT_STORE_SM_LAST_STATE__\0";


/* State values */
const unsigned short product_store_sm_state_values[]={
	Idle,MotorRunning,ReleasingOutletDoor,__PRODUCT_STORE_SM_LAST_STATE__
};


const unsigned short product_store_sm_state_idx[]={
	0,5,18,38,70};

#define PRODUCT_STORE_SM_EVENT_START 0


/* Event names */
const char product_store_sm_events[]=
	"evReleaseSelectedProduct\0evTimeout\0NO_MSG\0";


/* Event values */
const unsigned short product_store_sm_event_values[]={
	evReleaseSelectedProduct,evTimeout,PRODUCT_STORE_SM_NO_MSG
};


/* Helper to map event enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short product_store_sm_map_event_enum(unsigned short evt){
	unsigned short i;
	for(i=0; i<2;i++){
		if(product_store_sm_event_values[i]==evt) return i;
	}
	return 2; // should never be reached
}


/* Helper to map state enum value to an index starting from 0 */
/* Needed if the event enum value is not just 0,1,2,... */
unsigned short product_store_sm_map_state_enum(unsigned short state){
	unsigned short i;
	for(i=0; i<3;i++){
		if(product_store_sm_state_values[i]==state) return i;
	}
	return 3; // should never be reached
}



const unsigned short product_store_sm_evt_idx[]={
	0,25,35
};

const char* product_store_smGetNameByState(unsigned short state){
	unsigned short idx = product_store_sm_map_state_enum(state);
	return product_store_sm_states+product_store_sm_state_idx[idx];
}

const char* product_store_smGetNameByEvent(PRODUCT_STORE_SM_EVENT_T evt){
	unsigned short idx = product_store_sm_map_event_enum(evt);
	return product_store_sm_events+product_store_sm_evt_idx[idx];
}


#endif
