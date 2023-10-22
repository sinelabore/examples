/* (c) Peter Mueller, 2010 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven oven.cdd   */
/* This file is generated from oven.cdd - do not edit manually  */
/* Generated on: Wed Nov 24 21:11:29 CET 2010 / version 2.2 */


#include "mydefs.h"
#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include <stdio.h>
#include <bsp.h>

extern T_PWR pwr;
extern rtems_id Queue_id;

uint8_t msg=NO_MSG;
size_t received;
rtems_status_code status;


void   oven(OVEN_INSTANCEDATA_T *instanceVar){

	OVEN_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->superEntry==1U){
		ovenOff();

		instanceVar->superEntry=0U;
	}

	/* action code */
	/* wait for message */
	status = rtems_message_queue_receive(
	      Queue_id,
	      (void *) &msg,
	      &received,
	      RTEMS_DEFAULT_OPTIONS,
	      RTEMS_NO_TIMEOUT
	    );
	if ( status != RTEMS_SUCCESSFUL )
	  fputs( "receive did not work\n", stderr );


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
						ovenOff();
						timer_pause();


						/* adjust state variables  */
						instanceVar->stateVarSuper = CookingPause;
					}else if(msg==(OVEN_EVENT_T)evTimeout){
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
					if(msg==(OVEN_EVENT_T)evDoorOpen){
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
				
				if(instanceVar->stateVarSuper==Idle){


				}else if(instanceVar->stateVarSuper==Cooking){


				}else if(instanceVar->stateVarSuper==Completed){


				}else if(instanceVar->stateVarSuper==CookingPause){

				}


				/* Action code for transition  */
				timer_dec();

				instanceVar->stateVar = Super;/* entry chain  */
				if(instanceVar->stateVarSuper==Idle){
					ovenOff();
				}else if(instanceVar->stateVarSuper==Cooking){
					ovenOn();
				}else if(instanceVar->stateVarSuper==Completed){
				}else if(instanceVar->stateVarSuper==CookingPause){
				}

			}else if(msg==(OVEN_EVENT_T)evInc){
				/* Transition from Super to Super */
				evConsumed=1U;
				
				if(instanceVar->stateVarSuper==Idle){


				}else if(instanceVar->stateVarSuper==Cooking){


				}else if(instanceVar->stateVarSuper==Completed){


				}else if(instanceVar->stateVarSuper==CookingPause){

				}


				/* Action code for transition  */
				timer_inc();

				instanceVar->stateVar = Super;/* entry chain  */
				if(instanceVar->stateVarSuper==Idle){
					ovenOff();
				}else if(instanceVar->stateVarSuper==Cooking){
					ovenOn();
				}else if(instanceVar->stateVarSuper==Completed){
				}else if(instanceVar->stateVarSuper==CookingPause){
				}

			}else if(msg==(OVEN_EVENT_T)evPwr){
				/* Transition from Super to Super */
				evConsumed=1U;
				
				if(instanceVar->stateVarSuper==Idle){


				}else if(instanceVar->stateVarSuper==Cooking){


				}else if(instanceVar->stateVarSuper==Completed){


				}else if(instanceVar->stateVarSuper==CookingPause){

				}


				/* Action code for transition  */
				ovenSetPwr(pwr);

				instanceVar->stateVar = Super;/* entry chain  */
				if(instanceVar->stateVarSuper==Idle){
					ovenOff();
				}else if(instanceVar->stateVarSuper==Cooking){
					ovenOn();
				}else if(instanceVar->stateVarSuper==Completed){
				}else if(instanceVar->stateVarSuper==CookingPause){
				}

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
