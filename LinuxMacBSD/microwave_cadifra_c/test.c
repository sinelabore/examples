/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -c -p CADIFRA -o test first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually  */
/* Generated on: Mon Jan 06 19:13:28 CET 2014 / version 3.5 */


#include <stdint.h> // std. types
#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include <stdio.h>

extern OVEN_EVENT_T msg;
extern T_PWR pwr;
uint8_t status;

uint8_t initHardware(void){
   return 0; // just as demo
}

void  test(TEST_INSTANCEDATA_T *instanceVar){

	TEST_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->superEntry==1U){
		status=initHardware();
		if(status!=0){
			// signal error
		}else{
		ovenOff();
		}

		instanceVar->superEntry=0U;
	}

	/* action code */
	/* just a comment */


	switch (instanceVar->stateVar) {

		case Error:
		break; /* end of case Error  */

		case Super:

			switch (instanceVar->stateVarSuper) {

				case Idle:
					if(msg==(TEST_EVENT_T)evDoorClosed){
						if(timer_preset()==0){
							/* Transition from Idle to Idle */
							evConsumed=1U;


							/* adjust state variables  */
							instanceVar->stateVarSuper = Idle;
						}else{
							/* Transition from Idle to Cooking */
							evConsumed=1U;

							/* Action code for transition  */
							
							timer_start();


							/* OnEntry code of state Cooking */
							ovenOn();

							/* adjust state variables  */
							instanceVar->stateVarSuper = Cooking;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Idle  */

				case Cooking:
					if(msg==(TEST_EVENT_T)evDoorOpen){
						/* Transition from Cooking to CookingPause */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_pause();



						/* adjust state variables  */
						instanceVar->stateVarSuper = CookingPause;
					}else if(msg==(TEST_EVENT_T)evTimeout){
						/* Transition from Cooking to Completed */
						evConsumed=1U;

						/* Action code for transition  */
						ovenOff();
						timer_clear();



						/* adjust state variables  */
						instanceVar->stateVarSuper = Completed;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case Completed:
					if(msg==(TEST_EVENT_T)evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1U;


						/* adjust state variables  */
						instanceVar->stateVarSuper = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				case CookingPause:
					if(msg==(TEST_EVENT_T)evDoorClosed){
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

				if(msg==(TEST_EVENT_T)evDec){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_dec();


					instanceVar->stateVar = Super;/* entry chain  */
					if(instanceVar->stateVarSuper==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(TEST_EVENT_T)evInc){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					timer_inc();


					instanceVar->stateVar = Super;/* entry chain  */
					if(instanceVar->stateVarSuper==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else if(msg==(TEST_EVENT_T)evPwr){
					/* Transition from Super to Super */
					evConsumed=1U;
					
					/* Action code for transition  */
					ovenSetPwr(pwr);


					instanceVar->stateVar = Super;/* entry chain  */
					if(instanceVar->stateVarSuper==Cooking){
						ovenOn();

					}else {
						/* Intentionally left blank */
					};


				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Super  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void testResetMachine(TEST_INSTANCEDATA_T *instanceVar){
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

}
// Helper(s) to reset history
void testResetHistorySuper(TEST_INSTANCEDATA_T *instanceVar){
	instanceVar->stateVarSuper=Idle;
}


// Helper(s) to find out if the machine is in a certain state

uint8_t testIsInSuper(TEST_INSTANCEDATA_T *instanceVar){
	return(((instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t testIsInCompleted(TEST_INSTANCEDATA_T *instanceVar){
	return(((instanceVar->stateVarSuper== Completed)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t testIsInCookingPause(TEST_INSTANCEDATA_T *instanceVar){
	return(((instanceVar->stateVarSuper== CookingPause)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t testIsInIdle(TEST_INSTANCEDATA_T *instanceVar){
	return(((instanceVar->stateVarSuper== Idle)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t testIsInError(TEST_INSTANCEDATA_T *instanceVar){
	return(((instanceVar->stateVar== Error)) ? (1U) : (0U));
}

uint8_t testIsInCooking(TEST_INSTANCEDATA_T *instanceVar){
	return(((instanceVar->stateVarSuper== Cooking)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}


// Helper to get id of innermost active state
TEST_STATES_T testGetInnermostActiveState(TEST_INSTANCEDATA_T *instanceVar){
	if(testIsInCooking(instanceVar)){
		return Cooking;
	}else if(testIsInIdle(instanceVar)){
		return Idle;
	}else if(testIsInCookingPause(instanceVar)){
		return CookingPause;
	}else if(testIsInCompleted(instanceVar)){
		return Completed;
	}else if(testIsInError(instanceVar)){
		return Error;
	}else{
		return __TEST_LAST_STATE__;
	}
}

