/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p ssc -o test test.xml   */
/* This file is generated from test.xml - do not edit manually  */
/* Generated on: Sat Mar 13 15:08:00 CET 2021 / Version 5.0.6b1 */


#include <stdio.h>
#include <stdint.h>
#include "../lib/fifo.h"
#include "../lib/timer.h"
#include <test_ext.h>
#include <test.h>

#define T5S 50

extern FIFO_T testMachineFifoInst;
uint8_t idT1, idT2;



void  test(TEST_INSTANCEDATA_T *instanceVar, TEST_EVENT_T  msg){

	TEST_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->activeEntry==1U){
		// create cyclic timers
		idT1 = timerCreate(&testMachineFifoInst, evTimeoutA, true); //cyclic
		idT2 = timerCreate(&testMachineFifoInst, evTimeoutB, false);// one shot
		timerStart(idT1, T1S);
		timerStart(idT2, T5S);
		
		printf("Entering Active\n");
		

		instanceVar->activeEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case Active:

			switch (instanceVar->stateVarActive) {

				case On:
					if(msg==(TEST_EVENT_T)evTimeoutA){
						/* Transition from On to Off */
						evConsumed=1U;

						/* Action code for transition  */
						printf("Off\n");



						/* adjust state variables  */
						instanceVar->stateVarActive = Off;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case On  */

				case Off:
					if(msg==(TEST_EVENT_T)evTimeoutA){
						/* Transition from Off to On */
						evConsumed=1U;

						/* Action code for transition  */
						printf("On\n");



						/* adjust state variables  */
						instanceVar->stateVarActive = On;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Off  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Active */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(TEST_EVENT_T)evTimeoutB){
					/* Transition from Active to Inactive */
					evConsumed=1U;
					
					/* Action code for transition  */
					printf("Go Inactive\n");
					timerStart(idT2, T5S);


					/* OnEntry code of state Inactive */
					printf("Entering Inactive\n");

					/* adjust state variables  */
					instanceVar->stateVar = Inactive;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Active  */

		case Inactive:
			if(msg==(TEST_EVENT_T)evTimeoutA){
				evConsumed=1U;
				/* Action code for inner transition  */
				// ignore
				printf("ignore event\n");


			}else if(msg==(TEST_EVENT_T)evTimeoutB){
				/* Transition from Inactive to Active */
				evConsumed=1U;

				/* Action code for transition  */
				printf("Go Active\n");
				timerStart(idT2, T5S);


				/* OnEntry code of state Active */
				printf("Entering Active\n");
				instanceVar->stateVar = Active;/* Default in entry chain  */
				instanceVar->stateVarActive = Off;/* Default in entry chain  */

			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Inactive  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void testResetMachine(TEST_INSTANCEDATA_T * const instanceVar){
	instanceVar->activeEntry = 1U;
	instanceVar->stateVar = Active;
	instanceVar->stateVarActive = Off;

}
// Helper(s) to reset history
void testResetHistoryActive(TEST_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarActive=Off;
}


// Helper(s) to find out if the machine is in a certain state

uint8_t testIsInActive(const TEST_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t testIsInInactive(const TEST_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== Inactive)) ? (1U) : (0U));
}

uint8_t testIsInOff(const TEST_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarActive== Off)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}

uint8_t testIsInOn(const TEST_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarActive== On)&&(instanceVar->stateVar== Active)) ? (1U) : (0U));
}


// Helper to get id of innermost active state
TEST_STATES_T testGetInnermostActiveState(const TEST_INSTANCEDATA_T * const instanceVar){
	if(testIsInOn(instanceVar)){
		return On;
	}else if(testIsInOff(instanceVar)){
		return Off;
	}else if(testIsInInactive(instanceVar)){
		return Inactive;
	}else{
		return __TEST_LAST_STATE__;
	}
}

