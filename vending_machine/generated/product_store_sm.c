/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -l cx -v -p ASTAH -o product_store_sm -t machine:product_store_sm:sm ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sat Mar 13 15:41:53 CET 2021 / Version 5.0.6b1 */


#include <stdio.h>
#include <stdlib.h>

#include "product_store_sm_ext.h"
#include "product_store_sm.h"
#include "vending_machine_ext.h"
#include "lib/timer.h"
#include "lib/fifo.h"

uint8_t store_timer_id;
extern FIFO_T* fifo2Self;
extern FIFO_T fifo2VendingMachine;



void  product_store_sm(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar, PRODUCT_STORE_SM_EVENT_T  msg){
	
	/*execute entry code of default state once to init machine */
	if(instanceVar->idleEntry==1U){
		 store_timer_id = timerCreate(fifo2Self, evTimeout, 0);
		printf("ProdcutStore::   product store idle\n");
		

		instanceVar->idleEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case Idle:
			if(msg==(PRODUCT_STORE_SM_EVENT_T)evReleaseSelectedProduct){

				/* Transition from Idle to MotorRunning */
				/* OnEntry code of state MotorRunning */
				printf("ProdcutStore::  product store motor running\n");
				timerStart(store_timer_id, T1S);

				/* adjust state variables  */
				instanceVar->stateVar = MotorRunning;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Idle  */

		case MotorRunning:
			if(msg==(PRODUCT_STORE_SM_EVENT_T)evTimeout){
				/* OnExit code of state MotorRunning */
				printf("ProductStore::  product store motor stopped\n");

				/* Transition from MotorRunning to ReleasingOutletDoor */
				/* OnEntry code of state ReleasingOutletDoor */
				printf("ProdcutStore::  product store release outlet\n");
				timerStart(store_timer_id, T3S);
				

				/* adjust state variables  */
				instanceVar->stateVar = ReleasingOutletDoor;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case MotorRunning  */

		case ReleasingOutletDoor:
			if(msg==(PRODUCT_STORE_SM_EVENT_T)evTimeout){

				/* Transition from ReleasingOutletDoor to Idle */
				/* Action code for transition  */
				fifoPut(&fifo2VendingMachine, evReleaseDone);


				/* OnEntry code of state Idle */
				printf("ProdcutStore::   product store idle\n");

				/* adjust state variables  */
				instanceVar->stateVar = Idle;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case ReleasingOutletDoor  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void product_store_smResetMachine(PRODUCT_STORE_SM_INSTANCEDATA_T * const instanceVar){
	instanceVar->idleEntry = 1U;
	instanceVar->stateVar = Idle;
	
}

// Helper(s) to find out if the machine is in a certain state

uint8_t product_store_smIsInIdle(const PRODUCT_STORE_SM_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== Idle)) ? (1U) : (0U));
}

uint8_t product_store_smIsInMotorRunning(const PRODUCT_STORE_SM_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== MotorRunning)) ? (1U) : (0U));
}

uint8_t product_store_smIsInReleasingOutletDoor(const PRODUCT_STORE_SM_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== ReleasingOutletDoor)) ? (1U) : (0U));
}


// Helper to get id of innermost active state
PRODUCT_STORE_SM_STATES_T product_store_smGetInnermostActiveState(const PRODUCT_STORE_SM_INSTANCEDATA_T * const instanceVar){
	if(product_store_smIsInReleasingOutletDoor(instanceVar)){
		return ReleasingOutletDoor;
	}else if(product_store_smIsInMotorRunning(instanceVar)){
		return MotorRunning;
	}else if(product_store_smIsInIdle(instanceVar)){
		return Idle;
	}else{
		return __PRODUCT_STORE_SM_LAST_STATE__;
	}
}

