/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options:   */
/* This file is generated from sm.xml - do not edit manually  */
/* Generated on: Sun Oct 08 21:03:24 CEST 2023 / Version 6.1.3705 */


#include "main.h"
#include "cmsis_os.h"
#include "sm_ext.h"
#include "sm.h"

extern osTimerId_t cyclicTimer0Handle;




void  sm(SM_INSTANCEDATA_T *instanceVar, SM_EVENT_T  msg){

	SM_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->operationalEntry==1U){
		osTimerStart(cyclicTimer0Handle, 100 /* ms */); // start timer
		
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		

		instanceVar->operationalEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case Failsafe:
		break; /* end of case Failsafe  */

		case Operational:

			switch (instanceVar->stateVarOperational) {

				case On:
					if(msg==(SM_EVENT_T)evTimeout){
						/* Transition from On to Off */
						evConsumed=1U;

						/* OnEntry code of state Off */
						HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

						/* adjust state variables  */
						instanceVar->stateVarOperational = Off;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case On  */

				case Off:
					if(msg==(SM_EVENT_T)evTimeout){
						/* Transition from Off to On */
						evConsumed=1U;

						/* OnEntry code of state On */
						HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

						/* adjust state variables  */
						instanceVar->stateVarOperational = On;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Off  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Operational */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(SM_EVENT_T)evGoFailsafe){
					/* Transition from Operational to Failsafe */
					evConsumed=1U;
					
					/* OnEntry code of state Failsafe */
					osTimerStop(cyclicTimer0Handle); // stop timer
					HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

					/* adjust state variables  */
					instanceVar->stateVar = Failsafe;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Operational  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Initialize state machine */
void smInitMachine(SM_INSTANCEDATA_T * const instanceVar, SM_INST_ID_T instId) {
	instanceVar->operationalEntry = 1U;
	instanceVar->stateVar = Operational;
	instanceVar->stateVarOperational = Off;

	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void smResetMachine(SM_INSTANCEDATA_T * const instanceVar) {
	instanceVar->operationalEntry = 1U;
	instanceVar->stateVar = Operational;
	instanceVar->stateVarOperational = Off;

}
/* Helper(s) to reset history */
void smResetHistoryOperational(SM_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarOperational=Off;
}


/* Helper(s) to find out if the machine is in a certain state */

uint8_t smIsInFailsafe(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Failsafe)) ? (1U) : (0U));
}

uint8_t smIsInOff(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarOperational== Off)&&(instanceVar->stateVar== Operational)) ? (1U) : (0U));
}

uint8_t smIsInOn(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarOperational== On)&&(instanceVar->stateVar== Operational)) ? (1U) : (0U));
}

uint8_t smIsInOperational(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Operational)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
SM_STATES_T smGetInnermostActiveState(const SM_INSTANCEDATA_T * const instanceVar) {
	SM_STATES_T innermost;

	if(smIsInOn(instanceVar)) {
		innermost =On;
	}else if(smIsInOff(instanceVar)) {
		innermost = Off;
	}else if(smIsInFailsafe(instanceVar)) {
		innermost = Failsafe;
	}else{
		innermost = __SM_LAST_STATE__;
	}
	return innermost;	
}

