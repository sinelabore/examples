/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p ssc sm.xml   */
/* This file is generated from sm.xml - do not edit manually  */
/* Generated on: Fri Jul 19 18:23:30 CEST 2024 / Version 6.3.3. */


#include "RTOS.h"
#include <stdio.h>
#include <stdint.h>

#include "sm_ext.h"
#include "sm.h"
#include "../embos_setup/BSP.h"


extern OS_TIMER MyTimer;
uint8_t cnt;




void  sm(SM_INSTANCEDATA_T *instanceVar, SM_EVENT_T  msg){
	
	/*execute entry code of default state once to init machine */
	if(instanceVar->onEntry==1U){
		OS_RetriggerTimer(&MyTimer);
		BSP_SetLED(0);
		

		instanceVar->onEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case SLOW:
			if(msg==(SM_EVENT_T)evtTimeout){
				if(cnt>10){
					/* Transition from SLOW to FAST */

					/* Action code for transition  */
					OS_SetTimerPeriod(&MyTimer, 100);
					cnt=0;


					/* OnEntry code of state FAST */
					OS_RetriggerTimer(&MyTimer);
					BSP_ToggleLED(0);

					/* adjust state variables  */
					instanceVar->stateVar = FAST;
				}else{
					/* Transition from SLOW to SLOW */

					/* Action code for transition  */
					cnt++;


					/* OnEntry code of state SLOW */
					OS_RetriggerTimer(&MyTimer);
					BSP_ToggleLED(0);

					/* adjust state variables  */
					instanceVar->stateVar = SLOW;
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case SLOW  */

		case FAST:
			if(msg==(SM_EVENT_T)evtTimeout){
				if(cnt>10){
					/* Transition from FAST to ON */

					/* Action code for transition  */
					cnt=0;
					OS_SetTimerPeriod(&MyTimer, 2000);


					/* OnEntry code of state ON */
					OS_RetriggerTimer(&MyTimer);
					BSP_SetLED(0);

					/* adjust state variables  */
					instanceVar->stateVar = ON;
				}else{
					/* Transition from FAST to FAST */

					/* Action code for transition  */
					cnt++;


					/* OnEntry code of state FAST */
					OS_RetriggerTimer(&MyTimer);
					BSP_ToggleLED(0);

					/* adjust state variables  */
					instanceVar->stateVar = FAST;
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case FAST  */

		case ON:
			if(msg==(SM_EVENT_T)evtTimeout){
				/* Transition from ON to SLOW */

				/* Action code for transition  */
				OS_SetTimerPeriod(&MyTimer, 500);


				/* OnEntry code of state SLOW */
				OS_RetriggerTimer(&MyTimer);
				BSP_ToggleLED(0);

				/* adjust state variables  */
				instanceVar->stateVar = SLOW;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case ON  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Initialize state machine */
void smInitMachine(SM_INSTANCEDATA_T * const instanceVar, SM_INST_ID_T instId) {
	instanceVar->onEntry = 1U;
	instanceVar->stateVar = ON;
	
	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void smResetMachine(SM_INSTANCEDATA_T * const instanceVar) {
	instanceVar->onEntry = 1U;
	instanceVar->stateVar = ON;
	
}

/* Helper(s) to find out if the machine is in a certain state */

uint8_t smIsInFAST(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== FAST)) ? (1U) : (0U));
}

uint8_t smIsInON(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== ON)) ? (1U) : (0U));
}

uint8_t smIsInSLOW(const SM_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== SLOW)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
SM_STATES_T smGetInnermostActiveState(const SM_INSTANCEDATA_T * const instanceVar) {
	SM_STATES_T innermost;

	if(smIsInSLOW(instanceVar)) {
		innermost =SLOW;
	}else if(smIsInON(instanceVar)) {
		innermost = ON;
	}else if(smIsInFAST(instanceVar)) {
		innermost = FAST;
	}else{
		innermost = __SM_LAST_STATE__;
	}
	return innermost;	
}

