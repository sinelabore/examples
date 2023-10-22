/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cx -v -p ASTAH -o oven -t final:oven:machine oven_model.asml   */
/* This file is generated from oven_model.asml - do not edit manually  */
/* Generated on: Sun Oct 22 17:23:34 CEST 2023 / Version 6.1.3706 */


#include <stdint.h>
#include "oven_selftest.h"
#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include <stdio.h>

extern unsigned char msg;
extern T_PWR pwr;

uint8_t hwStatus;




void  oven(OVEN_INSTANCEDATA_T *instanceVar){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->superEntry==1U){
		hwStatus=selftest();
		if(hwStatus!=0){
			instanceVar->stateVar = Error;
		}else{
			instanceVar->stateVar = Super;
		}

		instanceVar->superEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case Super:

			switch (instanceVar->stateVarSuper) {

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
							instanceVar->stateVarSuper = Cooking;
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
						ovenOff(); timer_pause();



						/* adjust state variables  */
						instanceVar->stateVarSuper = CookingPause;
					}else if(msg==(OVEN_EVENT_T)evTimeout){
						/* Transition from Cooking to Completed */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff(); timer_clear();



						/* adjust state variables  */
						instanceVar->stateVarSuper = Completed;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case Completed:
					if(msg==(OVEN_EVENT_T)eDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1U;


						/* adjust state variables  */
						instanceVar->stateVarSuper = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				case CookingPause:
					if(msg==(OVEN_EVENT_T)evDoorClosed){
						/* Transition from CookingPause to Cooking */
						evConsumed=1U;

						/* Action code for transition  */
						timer_cont();


						/* OnEntry code of state Cooking */
						ovenOn();

						/* adjust state variables  */
						instanceVar->stateVarSuper = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Super */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(OVEN_EVENT_T)evDec){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_dec();


					instanceVar->stateVar = Super;/* Default in entry chain  */
					instanceVar->stateVarSuper = Idle;/* Default in entry chain  */

				}else if(msg==(OVEN_EVENT_T)evInc){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_inc();


					instanceVar->stateVar = Super;/* Default in entry chain  */
					instanceVar->stateVarSuper = Idle;/* Default in entry chain  */

				}else if(msg==(OVEN_EVENT_T)evPwr){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					ovenSetPwr(pwr);


					instanceVar->stateVar = Super;/* Default in entry chain  */
					instanceVar->stateVarSuper = Idle;/* Default in entry chain  */

				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Super  */

		case Error:
		break; /* end of case Error  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Initialize state machine */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

}
/* Helper(s) to reset history */
void ovenResetHistorySuper(OVEN_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarSuper=Idle;
}


/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInSuper(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Idle)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Cooking)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Completed)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== CookingPause)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInError(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Error)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar) {
	OVEN_STATES_T innermost;

	if(ovenIsInCookingPause(instanceVar)) {
		innermost =CookingPause;
	}else if(ovenIsInCompleted(instanceVar)) {
		innermost = Completed;
	}else if(ovenIsInCooking(instanceVar)) {
		innermost = Cooking;
	}else if(ovenIsInIdle(instanceVar)) {
		innermost = Idle;
	}else if(ovenIsInError(instanceVar)) {
		innermost = Error;
	}else{
		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;	
}

