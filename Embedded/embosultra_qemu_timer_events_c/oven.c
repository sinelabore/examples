/* (C) Your company *//* Command line options: -p ssc -l cx -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Tue Nov 17 19:36:59 CET 2026 / 8.0 */


#include <stdint.h>
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include <stdio.h>




OVEN_EV_CONSUMED_FLAG_T oven(OVEN_INSTANCEDATA_T *instanceVar, const OVEN_MSG_T *userData){

	OVEN_EV_CONSUMED_FLAG_T  evConsumed = 0U;


	
	/*execute entry code of default state once to init machine */
	if(instanceVar->superEntry==1U) {

		ovenOff();
		

		instanceVar->superEntry=0U;
	}


	/* action code */
	/* just a comment */


	switch (instanceVar->stateVar) {

		case Super:

			switch (instanceVar->stateVarSuper) {

				case Completed:
					if(userData->msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1U;

						/* OnEntry code of state Idle */
						ovenOff();

						/* adjust state variables  */
						instanceVar->stateVarSuper = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				case Cooking:
					if((userData->msg==(OVEN_EVENT_T)Cooking_0Timeout)&&(instanceVar->timer[OVEN_TM_COOKING_0]==TS_RUNNING)){
						instanceVar->timer[OVEN_TM_COOKING_0] = TS_RUNNING; /* every(1 s) taken */
						evConsumed=16U;
						/* Action code for inner transition  */
						printf(".");
						fflush(stdout);


					}else if((userData->msg==(OVEN_EVENT_T)cookTimerTimeout)&&(instanceVar->timer[OVEN_TM_COOKTIMER]==TS_RUNNING)){
						tsStop(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER); /* cookTimer: timeout taken, also when sent by hand */
						instanceVar->timer[OVEN_TM_COOKTIMER] = TS_IDLE;
						/* Transition from Cooking to Completed */
						evConsumed=16U;
						/* OnExit code of state Cooking */
						instanceVar->timerTime[OVEN_TM_COOKTIMER] = tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER); /* cookTimer keeps its time left */
						tsStop(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER);
						instanceVar->timer[OVEN_TM_COOKTIMER] = TS_IDLE;
						tsStop(instanceVar->timerService, instanceVar, OVEN_TM_COOKING_0);
						instanceVar->timer[OVEN_TM_COOKING_0] = TS_IDLE;

						/* Action code for transition  */
						ovenOff();



						/* adjust state variables  */
						instanceVar->stateVarSuper = Completed;
					}else if(userData->msg==(OVEN_EVENT_T)evDoorOpen){
						/* Transition from Cooking to CookingPause */
						evConsumed=1U;
						/* OnExit code of state Cooking */
						instanceVar->timerTime[OVEN_TM_COOKTIMER] = tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER); /* cookTimer keeps its time left */
						tsStop(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER);
						instanceVar->timer[OVEN_TM_COOKTIMER] = TS_IDLE;
						tsStop(instanceVar->timerService, instanceVar, OVEN_TM_COOKING_0);
						instanceVar->timer[OVEN_TM_COOKING_0] = TS_IDLE;

						/* Action code for transition  */
						ovenOff();



						/* adjust state variables  */
						instanceVar->stateVarSuper = CookingPause;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case CookingPause:
					if(userData->msg==(OVEN_EVENT_T)evDoorClosed){
						/* Transition from CookingPause to Cooking */
						evConsumed=1U;

						/* OnEntry code of state Cooking */
						ovenOn();
						instanceVar->timer[OVEN_TM_COOKTIMER] = TS_RUNNING;
						(void)tsStart(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER, instanceVar->timerTime[OVEN_TM_COOKTIMER], TS_ONCE); /* cookTimer: after() */
						instanceVar->timer[OVEN_TM_COOKING_0] = TS_RUNNING;
						(void)tsStart(instanceVar->timerService, instanceVar, OVEN_TM_COOKING_0, TS_S(1U), TS_EVERY); /* every(1 s) */

						/* adjust state variables  */
						instanceVar->stateVarSuper = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				case Idle:
					if(userData->msg==(OVEN_EVENT_T)evDoorClosed){
						if(((instanceVar->timer[OVEN_TM_COOKTIMER] != TS_IDLE) ? tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER) : instanceVar->timerTime[OVEN_TM_COOKTIMER]) > 0U){
							/* Transition from Idle to Cooking */
							evConsumed=1U;

							/* OnEntry code of state Cooking */
							ovenOn();
							instanceVar->timer[OVEN_TM_COOKTIMER] = TS_RUNNING;
							(void)tsStart(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER, instanceVar->timerTime[OVEN_TM_COOKTIMER], TS_ONCE); /* cookTimer: after() */
							instanceVar->timer[OVEN_TM_COOKING_0] = TS_RUNNING;
							(void)tsStart(instanceVar->timerService, instanceVar, OVEN_TM_COOKING_0, TS_S(1U), TS_EVERY); /* every(1 s) */

							/* adjust state variables  */
							instanceVar->stateVarSuper = Cooking;
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Idle  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Super */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(userData->msg==(OVEN_EVENT_T)evDec){
					evConsumed=1U;
					/* Action code for inner transition  */
					/* cookTimer: decrement(1s) */
					{
						const TS_TICKS_T smTicks = TS_S(1U);
						if(!tsSetTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER, tsTimeSub(tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER), smTicks))){ /* does not run: the time it keeps */
							instanceVar->timerTime[OVEN_TM_COOKTIMER] = tsTimeSub(instanceVar->timerTime[OVEN_TM_COOKTIMER], smTicks);
						}
					}
					ovenDisplay(((instanceVar->timer[OVEN_TM_COOKTIMER] != TS_IDLE) ? tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER) : instanceVar->timerTime[OVEN_TM_COOKTIMER]));


				}else if(userData->msg==(OVEN_EVENT_T)evInc){
					evConsumed=1U;
					/* Action code for inner transition  */
					/* cookTimer: increment(1s) */
					{
						const TS_TICKS_T smTicks = TS_S(1U);
						if(!tsSetTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER, tsTimeAdd(tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER), smTicks))){ /* does not run: the time it keeps */
							instanceVar->timerTime[OVEN_TM_COOKTIMER] = tsTimeAdd(instanceVar->timerTime[OVEN_TM_COOKTIMER], smTicks);
						}
					}
					ovenDisplay(((instanceVar->timer[OVEN_TM_COOKTIMER] != TS_IDLE) ? tsTimeLeft(instanceVar->timerService, instanceVar, OVEN_TM_COOKTIMER) : instanceVar->timerTime[OVEN_TM_COOKTIMER]));


				}else if(userData->msg==(OVEN_EVENT_T)evPwr){
					uint16_t const pwr = userData->data.evPwr.pwr;
					evConsumed=1U;
					/* Action code for inner transition  */
					ovenSetPwr(pwr);


				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Super  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	/* Return flag if an event was processed */
	return evConsumed;
}

/* Initialize state machine */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

	instanceVar->inst_id = instId;
	/* Timers (UseTimerService) */
	{
		uint16_t timerId;
		for(timerId=0U; timerId<(uint16_t)OVEN_TM_COUNT; timerId++){
			instanceVar->timer[timerId] = TS_IDLE;
		}
		for(timerId=0U; timerId<(uint16_t)OVEN_TM_NAMED_COUNT; timerId++){
			instanceVar->timerTime[timerId] = 0U;
		}
	}

}

/* Implementation of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	instanceVar->stateVarSuper = Idle;

	/* Timers (UseTimerService) */
	{
		uint16_t timerId;
		tsStopAll(instanceVar->timerService, instanceVar);
		for(timerId=0U; timerId<(uint16_t)OVEN_TM_COUNT; timerId++){
			instanceVar->timer[timerId] = TS_IDLE;
		}
		for(timerId=0U; timerId<(uint16_t)OVEN_TM_NAMED_COUNT; timerId++){
			instanceVar->timerTime[timerId] = 0U;
		}
	}
}

/* The timeout event of a timer id (TimeoutsAsEvents). */
OVEN_EVENT_T ovenTimeoutEvent(uint16_t id) {
	static const OVEN_EVENT_T events[OVEN_TM_COUNT] = {
		cookTimerTimeout, /* OVEN_TM_COOKTIMER */
		Cooking_0Timeout /* OVEN_TM_COOKING_0 */
	};
	return (id < (uint16_t)OVEN_TM_COUNT) ? events[id] : OVEN_NO_MSG;
}
/* Helper(s) to reset history */
void ovenResetHistorySuper(OVEN_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarSuper=Idle;
}

/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Completed)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Cooking)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== CookingPause)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarSuper== Idle)&&(instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInSuper(const OVEN_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== Super)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar) {
    	OVEN_STATES_T innermost;

	if(ovenIsInIdle(instanceVar)) {
    		innermost = Idle;
	} else if(ovenIsInCookingPause(instanceVar)) {
    		innermost = CookingPause;
	} else if(ovenIsInCooking(instanceVar)) {
    		innermost = Cooking;
	} else if(ovenIsInCompleted(instanceVar)) {
    		innermost = Completed;
	} else {
    		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;
}


