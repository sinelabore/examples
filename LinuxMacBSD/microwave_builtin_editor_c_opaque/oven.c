/* (C) Sinelabore Software Tools GmbH */
/* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Mon Apr 01 18:02:43 CEST 2024 / Version 6.3.2.3814 */


#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mydefs.h"
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"


extern T_PWR pwr;



/* Region handler prototypes  */
static OVEN_EV_CONSUMED_FLAG_T ovenMainRegion(const struct OVEN_INSTANCEDATA * const instanceVar, struct OVEN_INSTANCEDATA *instanceVarCopy, OVEN_EVENT_T msg);

void  oven(struct OVEN_INSTANCEDATA *instanceVar, OVEN_EVENT_T  msg){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;


	/* Create a copy of the instance data.
	   Changes during the machine execution are done on the copy 
	   while tests (e.g. isIn) must use the unmodified instance data */
	struct OVEN_INSTANCEDATA instanceVarCopy = *instanceVar;
	// test
	/*execute entry code of default state once to init machine */
	if((&instanceVarCopy)->superEntry==1U){
		ovenOff();
		

		(&instanceVarCopy)->superEntry=0U;
	}

	/* action code */
	/* just a comment */


	switch (instanceVar->stateVar) {

		case Super:
			/* calling region code  */
			evConsumed |= ovenMainRegion(instanceVar, &instanceVarCopy, msg);

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(OVEN_EVENT_T)evDec){
					/* Transition from Super to Super */

					/* Exit code for regions in state Super */
					
					/* Action code for transition  */
					timer_dec();



					/* Entry code for regions in state Super */
					ovenOff();
					/* Default in entry chain  */
					(&instanceVarCopy)->stateVarMainRegion = Idle;/* Default in entry chain  */


					/* adjust state variables  */
					(&instanceVarCopy)->stateVar = Super;
				}else if(msg==(OVEN_EVENT_T)evInc){
					/* Transition from Super to Super */

					/* Exit code for regions in state Super */
					
					/* Action code for transition  */
					timer_inc();



					/* Entry code for regions in state Super */
					ovenOff();
					/* Default in entry chain  */
					(&instanceVarCopy)->stateVarMainRegion = Idle;/* Default in entry chain  */


					/* adjust state variables  */
					(&instanceVarCopy)->stateVar = Super;
				}else if(msg==(OVEN_EVENT_T)evPwr){
					/* Transition from Super to Super */

					/* Exit code for regions in state Super */
					
					/* Action code for transition  */
					ovenSetPwr(pwr);



					/* Entry code for regions in state Super */
					ovenOff();
					/* Default in entry chain  */
					(&instanceVarCopy)->stateVarMainRegion = Idle;/* Default in entry chain  */


					/* adjust state variables  */
					(&instanceVarCopy)->stateVar = Super;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Super  */

		default:
			// test unknown state handler
		break;
	} /* end switch stateVar_root */

	/* Save the modified instance data */
	*instanceVar = instanceVarCopy;
}


/* Get size of instance variable. */
size_t ovenSizeOf(void){
	return sizeof(struct OVEN_INSTANCEDATA );
}


/* Initialize state machine */
void ovenInitMachine(struct OVEN_INSTANCEDATA * const instanceVar, OVEN_INST_ID_T instId) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	
	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void ovenResetMachine(struct OVEN_INSTANCEDATA * const instanceVar) {
	instanceVar->superEntry = 1U;
	instanceVar->stateVar = Super;
	
}

/* Helper(s) to find out if the machine is in a certain state */

uint8_t ovenIsInSuper(const struct OVEN_INSTANCEDATA * const instanceVar) {
	return(((instanceVar->stateVar== Super)) ? (1U) : (0U));
}

uint8_t ovenIsInCompleted(const struct OVEN_INSTANCEDATA * const instanceVar) {
	return(((instanceVar->stateVarMainRegion== Completed) && (ovenIsInSuper(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInCooking(const struct OVEN_INSTANCEDATA * const instanceVar) {
	return(((instanceVar->stateVarMainRegion== Cooking) && (ovenIsInSuper(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInCookingPause(const struct OVEN_INSTANCEDATA * const instanceVar) {
	return(((instanceVar->stateVarMainRegion== CookingPause) && (ovenIsInSuper(instanceVar))) ? (1U) : (0U));
}

uint8_t ovenIsInIdle(const struct OVEN_INSTANCEDATA * const instanceVar) {
	return(((instanceVar->stateVarMainRegion== Idle) && (ovenIsInSuper(instanceVar))) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
OVEN_STATES_T ovenGetInnermostActiveState(const struct OVEN_INSTANCEDATA * const instanceVar) {
	OVEN_STATES_T innermost;

	if(ovenIsInSuper(instanceVar)) {
		innermost =Super;
	}else{
		innermost = __OVEN_LAST_STATE__;
	}
	return innermost;	
}



/* Region code for state MainRegion */

static OVEN_EV_CONSUMED_FLAG_T ovenMainRegion(const struct OVEN_INSTANCEDATA * const instanceVar, struct OVEN_INSTANCEDATA *instanceVarCopy, OVEN_EVENT_T msg){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	switch (instanceVar->stateVarMainRegion) {

		case Completed:
			if(msg==(OVEN_EVENT_T)evDoorOpen){
				/* Transition from Completed to Idle */
				evConsumed=1U;

				/* OnEntry code of state Idle */
				ovenOff();

				/* adjust state variables  */
				instanceVarCopy->stateVarMainRegion = Idle;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Completed  */

		case Cooking:
			if(msg==(OVEN_EVENT_T)evDoorOpen){
				/* Transition from Cooking to CookingPause */
				evConsumed=1U;

				/* Action code for transition  */
				ovenOff();
				timer_pause();



				/* adjust state variables  */
				instanceVarCopy->stateVarMainRegion = CookingPause;
			}else if(msg==(OVEN_EVENT_T)evTimeout){
				/* Transition from Cooking to Completed */
				evConsumed=1U;

				/* Action code for transition  */
				ovenOff();
				timer_clear();



				/* adjust state variables  */
				instanceVarCopy->stateVarMainRegion = Completed;
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
				instanceVarCopy->stateVarMainRegion = Cooking;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case CookingPause  */

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
					instanceVarCopy->stateVarMainRegion = Cooking;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Idle  */

		default:
			// test unknown state handler
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}


