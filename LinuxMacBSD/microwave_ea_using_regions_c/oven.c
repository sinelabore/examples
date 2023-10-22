/* (C) Your company*//* Command line options: -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Oct 22 18:01:32 CEST 2023 / Version 6.1.3706 */


#include "oven_ext.h"
#include "oven_hlp.h"
#include "oven.h"
#include <stdio.h>

extern unsigned char msg;
extern T_PWR pwr;



/* Region handler prototypes  */
static OVEN_EV_CONSUMED_FLAG_T ovenLight(const OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INSTANCEDATA_T *instanceVarCopy, OVEN_EVENT_T msg);
static OVEN_EV_CONSUMED_FLAG_T ovenPower(const OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INSTANCEDATA_T *instanceVarCopy, OVEN_EVENT_T msg);
static OVEN_EV_CONSUMED_FLAG_T ovenRadioator(const OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INSTANCEDATA_T *instanceVarCopy, OVEN_EVENT_T msg);

void  oven(OVEN_INSTANCEDATA_T *instanceVar){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;


	/* Create a copy of the instance data.
	   Changes during the machine execution are done on the copy 
	   while tests (e.g. isIn) must use the unmodified instance data */
	OVEN_INSTANCEDATA_T instanceVarCopy = *instanceVar;
	

	switch (instanceVar->stateVar) {

		case Active:
			/* calling region code */
			evConsumed |= ovenLight(instanceVar, &instanceVarCopy, msg);
			evConsumed |= ovenPower(instanceVar, &instanceVarCopy, msg);
			evConsumed |= ovenRadioator(instanceVar, &instanceVarCopy, msg);

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(OVEN_EVENT_T)evDec){
					/* Transition from Active to Active */

					/* Exit code for regions in state Active */
					
					/* Action code for transition  */
					timer_dec();



					/* Entry code for regions in state Active */
					/* entry chain  */
					/* entry chain  */
					/* entry chain  */


					/* adjust state variables  */
					(&instanceVarCopy)->stateVar = Active;
				}else if(msg==(OVEN_EVENT_T)evInc){
					/* Transition from Active to Active */

					/* Exit code for regions in state Active */
					
					/* Action code for transition  */
					timer_inc();



					/* Entry code for regions in state Active */
					/* entry chain  */
					/* entry chain  */
					/* entry chain  */


					/* adjust state variables  */
					(&instanceVarCopy)->stateVar = Active;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Active  */

		case Inactive:
			if(1){
				/* Transition from Inactive to Active */


				/* Entry code for regions in state Active */
				/* entry chain  */
				/* entry chain  */
				/* entry chain  */


				/* adjust state variables  */
				(&instanceVarCopy)->stateVar = Active;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Inactive  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	/* Save the modified instance data */
	*instanceVar = instanceVarCopy;
}

/* Initialize state machine */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId) {
	instanceVar->inactiveEntry = 1U;
	instanceVar->stateVar = Inactive;
	
	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar) {
	instanceVar->inactiveEntry = 1U;
	instanceVar->stateVar = Inactive;
	
}

/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInInactive(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Inactive)) ? (1U) : (0U));
}

uint8_t ovenIsInActive(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInLightOn(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarLight== LightOn) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInLightOff(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarLight== LightOff) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInHighPower(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarPower== HighPower) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInLowPower(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarPower== LowPower) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarRadioator== CookingPause) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarRadioator== Cooking) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInRadiatorOff(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarRadioator== RadiatorOff) && (ovenIsInActive(instanceVar))) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar) {
	OVEN_STATES_T innermost;

	if(ovenIsInActive(instanceVar)) {
		innermost =Active;
	}else if(ovenIsInInactive(instanceVar)) {
		innermost = Inactive;
	}else{
		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;	
}



/* Region code for state Light */

static OVEN_EV_CONSUMED_FLAG_T ovenLight(const OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INSTANCEDATA_T *instanceVarCopy, OVEN_EVENT_T msg){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	switch (instanceVar->stateVarLight) {

		case LightOff:
			if(msg==(OVEN_EVENT_T)evDoorClosed){
				if(timer_preset()>0){
					/* Transition from LightOff to LightOn */
					evConsumed=1U;

					/* Action code for transition  */
					switchLight(L_ON);



					/* adjust state variables  */
					instanceVarCopy->stateVarLight = LightOn;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LightOff  */

		case LightOn:
			if(msg==(OVEN_EVENT_T)evDoorOpen){
				if(timer_preset()==0){
					/* Transition from LightOn to LightOff */
					evConsumed=1U;

					/* Action code for transition  */
					switchLight(L_OFF);



					/* adjust state variables  */
					instanceVarCopy->stateVarLight = LightOff;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LightOn  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}




/* Region code for state Power */

static OVEN_EV_CONSUMED_FLAG_T ovenPower(const OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INSTANCEDATA_T *instanceVarCopy, OVEN_EVENT_T msg){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	switch (instanceVar->stateVarPower) {

		case HighPower:
			if(msg==(OVEN_EVENT_T)evPwrLow){
				/* Transition from HighPower to LowPower */
				evConsumed=1U;

				/* Action code for transition  */
				ovenSetPwr(PWR_LOW);



				/* adjust state variables  */
				instanceVarCopy->stateVarPower = LowPower;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case HighPower  */

		case LowPower:
			if(msg==(OVEN_EVENT_T)evPwrHigh){
				/* Transition from LowPower to HighPower */
				evConsumed=1U;

				/* Action code for transition  */
				ovenSetPwr(PWR_HIGH);



				/* adjust state variables  */
				instanceVarCopy->stateVarPower = HighPower;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LowPower  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}




/* Region code for state Radioator */

static OVEN_EV_CONSUMED_FLAG_T ovenRadioator(const OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INSTANCEDATA_T *instanceVarCopy, OVEN_EVENT_T msg){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	switch (instanceVar->stateVarRadioator) {

		case Cooking:
			if(msg==(OVEN_EVENT_T)evDoorOpen){
				/* Transition from Cooking to CookingPause */
				evConsumed=1U;

				/* Action code for transition  */
				ovenOff();
				timer_pause();



				/* adjust state variables  */
				instanceVarCopy->stateVarRadioator = CookingPause;
			}else if(msg==(OVEN_EVENT_T)evTimeout){
				/* Transition from Cooking to RadiatorOff */
				evConsumed=1U;

				/* Action code for transition  */
				ovenOff();
				timer_clear();



				/* adjust state variables  */
				instanceVarCopy->stateVarRadioator = RadiatorOff;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Cooking  */

		case CookingPause:
			if(msg==(OVEN_EVENT_T)evDoorClosed){
				/* Transition from CookingPause to Cooking */
				evConsumed=1U;

				/* Action code for transition  */
				timer_cont();



				/* adjust state variables  */
				instanceVarCopy->stateVarRadioator = Cooking;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case CookingPause  */

		case RadiatorOff:
			if(msg==(OVEN_EVENT_T)evDoorClosed){
				if(timer_preset()>0){
					/* Transition from RadiatorOff to Cooking */
					evConsumed=1U;

					/* Action code for transition  */
					timer_start();



					/* adjust state variables  */
					instanceVarCopy->stateVarRadioator = Cooking;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case RadiatorOff  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}


