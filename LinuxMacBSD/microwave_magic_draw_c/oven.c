/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cx -p md -t Model:oven_package:oven -o oven oven.xmi.xml   */
/* This file is generated from oven.xmi.xml - do not edit manually  */
/* Generated on: Sun Oct 22 18:11:21 CEST 2023 / Version 6.1.3706 */


#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include <stdio.h>

extern unsigned char msg;
extern T_PWR pwr;




void  oven(OVEN_INSTANCEDATA_T *instanceVar){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->activeEntry==1U){
		ovenOff();
		

		instanceVar->activeEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case Active:

			switch (instanceVar->stateVarActive) {

				case Idle:
					if(msg==(OVEN_EVENT_T)evDoorClosed){
						if(timer_preset()>0){
							/* Transition from Idle to Cooking */
							evConsumed=1U;

							/* Action code for transition  */
							timer_start();


							/* OnEntry code of state Cooking */
							ovenOn();

							/* adjust state variables  */
							instanceVar->stateVarActive = Cooking;
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Idle  */

				case Cooking:
					if(msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Cooking to CookingPause */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_pause();



						/* adjust state variables  */
						instanceVar->stateVarActive = CookingPause;
					}else if(msg==(OVEN_EVENT_T)evTimeout){
						/* Transition from Cooking to Completed */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_clear();



						/* adjust state variables  */
						instanceVar->stateVarActive = Completed;
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


						/* OnEntry code of state Cooking */
						ovenOn();

						/* adjust state variables  */
						instanceVar->stateVarActive = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				case Completed:
					if(msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1U;

						/* OnEntry code of state Idle */
						ovenOff();

						/* adjust state variables  */
						instanceVar->stateVarActive = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Active */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(OVEN_EVENT_T)evDec){
					/* Transition from Active to Active */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_dec();


					instanceVar->stateVar = Active;/* entry chain  */
					if(instanceVar->stateVarActive==Idle){
						ovenOff();

					}else if(instanceVar->stateVarActive==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(OVEN_EVENT_T)evInc){
					/* Transition from Active to Active */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_inc();


					instanceVar->stateVar = Active;/* entry chain  */
					if(instanceVar->stateVarActive==Idle){
						ovenOff();

					}else if(instanceVar->stateVarActive==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(OVEN_EVENT_T)evPwr){
					/* Transition from Active to Active */
					evConsumed=1U;
					
					/* Action code for transition  */
					 ovenSetPwr(pwr);


					instanceVar->stateVar = Active;/* entry chain  */
					if(instanceVar->stateVarActive==Idle){
						ovenOff();

					}else if(instanceVar->stateVarActive==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Active  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	/* Post Action Code */
	// only a test
	//
}

/* Initialize state machine */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId) {
	instanceVar->activeEntry = 1U;
	instanceVar->stateVar = Active;
	instanceVar->stateVarActive = Idle;

	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar) {
	instanceVar->activeEntry = 1U;
	instanceVar->stateVar = Active;
	instanceVar->stateVarActive = Idle;

}
/* Helper(s) to reset history */
void ovenResetHistoryActive(OVEN_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarActive=Idle;
}


/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInActive(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarActive== Idle)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarActive== Cooking)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarActive== CookingPause)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarActive== Completed)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar) {
	OVEN_STATES_T innermost;

	if(ovenIsInCompleted(instanceVar)) {
		innermost =Completed;
	}else if(ovenIsInCookingPause(instanceVar)) {
		innermost = CookingPause;
	}else if(ovenIsInCooking(instanceVar)) {
		innermost = Cooking;
	}else if(ovenIsInIdle(instanceVar)) {
		innermost = Idle;
	}else{
		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;	
}

