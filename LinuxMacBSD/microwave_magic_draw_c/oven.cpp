/* (c) Peter Mueller, 2010 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -l cppx -p md -t Model:oven_package:oven -o oven oven.xmi.xml   */
/* This file is generated from oven.xmi.xml - do not edit manually  */
/* Generated on: Tue Apr 05 19:26:41 CEST 2011 / version 2.36 */


#include "oven.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include <stdio.h>

extern unsigned char msg;
extern T_PWR pwr;
oven::oven(void)
{
	m_initialized=0U;
}



/* State names */
const char states[]=
	"Completed\0CookingPause\0Active\0Idle\0Cooking\0";

const unsigned short state_idx[]={
	0,10,23,30,35,43};

/* Event names */
const char events[]=
	"evDoorOpen\0evDec\0evTimeout\0evPwr\0evDoorClosed\0evInc\0NO_MSG\0";

const unsigned short evt_idx[]={
	0,11,17,27,33,46,52};

const char* oven::getNameByState(unsigned short state){
	return states+state_idx[state];
}

const char* oven::getNameByEvent(OVEN_EVENT_T evt){
	return events+evt_idx[evt];
}


// Helper(s) to reset history
void oven::resetHistoryActive(void){
	stateVarActive=Idle;
}


// Helper(s) to find out if the machine is in a certain state

int oven::isInCompleted(void){return(((stateVarActive== Completed)&&(stateVar== Active)) ? (true) : (false));}
int oven::isInCookingPause(void){return(((stateVarActive== CookingPause)&&(stateVar== Active)) ? (true) : (false));}
int oven::isInActive(void){return(((stateVar== Active)) ? (true) : (false));}
int oven::isInIdle(void){return(((stateVarActive== Idle)&&(stateVar== Active)) ? (true) : (false));}
int oven::isInCooking(void){return(((stateVarActive== Cooking)&&(stateVar== Active)) ? (true) : (false));}

// Initialize method. Must be called once to init the machine

void oven::initialize(){	//call on entry code of default states
	if(m_initialized==0U){

		m_initialized=1U;
		//call on entry code of default states
		ovenOff();

		// Set state vars to default states
		stateVar = Active; /* set init state of top state */
		stateVarActive = Idle; /* set init state of Active */
	}

}

// State machine event handler
int oven::processEvent(OVEN_EVENT_T msg){

	int evConsumed = 0;

	

	if(m_initialized==false) return 0;


	switch (stateVar) {

		case Active:

			switch (stateVarActive) {

				case Idle:
					if(msg==evDoorClosed){
						if(timer_preset()>0){
							/* Transition from Idle to Cooking */
							evConsumed=1;

							/* Action code for transition  */
							timer_start();

							/* OnEntry code of state Cooking */
							ovenOn();

							/* adjust state variables  */
							stateVarActive = Cooking;
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Idle  */

				case Cooking:
					if(msg==evDoorOpen){
						/* Transition from Cooking to CookingPause */
						evConsumed=1;

						/* Action code for transition  */
						ovenOff();
						timer_pause();


						/* adjust state variables  */
						stateVarActive = CookingPause;
					}else if(msg==evTimeout){
						/* Transition from Cooking to Completed */
						evConsumed=1;

						/* Action code for transition  */
						ovenOff();
						timer_clear();


						/* adjust state variables  */
						stateVarActive = Completed;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case CookingPause:
					if(msg==evDoorClosed){
						/* Transition from CookingPause to Cooking */
						evConsumed=1;

						/* Action code for transition  */
						timer_cont();

						/* OnEntry code of state Cooking */
						ovenOn();

						/* adjust state variables  */
						stateVarActive = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				case Completed:
					if(msg==evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed=1;

						/* OnEntry code of state Idle */
						ovenOff();

						/* adjust state variables  */
						stateVarActive = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Active */

		/* Check if event was already processed  */
		if(evConsumed==0){

			if(msg==evDec){
				/* Transition from Active to Active */
				evConsumed=1;
				
				if(stateVarActive== Idle){
					

				}else if(stateVarActive== Cooking){
					

				}else if(stateVarActive== CookingPause){
					

				}else if(stateVarActive== Completed){
					
				}

				/* Action code for transition  */
				timer_dec();

				stateVar = Active;/* entry chain  */
				if(stateVarActive== Idle){
					ovenOff();

				}else if(stateVarActive== Cooking){
					ovenOn();

				}else if(stateVarActive== CookingPause){
					
				}else if(stateVarActive== Completed){
					
				}

			}else if(msg==evInc){
				/* Transition from Active to Active */
				evConsumed=1;
				
				if(stateVarActive== Idle){
					

				}else if(stateVarActive== Cooking){
					

				}else if(stateVarActive== CookingPause){
					

				}else if(stateVarActive== Completed){
					
				}

				/* Action code for transition  */
				timer_inc();

				stateVar = Active;/* entry chain  */
				if(stateVarActive== Idle){
					ovenOff();

				}else if(stateVarActive== Cooking){
					ovenOn();

				}else if(stateVarActive== CookingPause){
					
				}else if(stateVarActive== Completed){
					
				}

			}else if(msg==evPwr){
				/* Transition from Active to Active */
				evConsumed=1;
				
				if(stateVarActive== Idle){
					

				}else if(stateVarActive== Cooking){
					

				}else if(stateVarActive== CookingPause){
					

				}else if(stateVarActive== Completed){
					
				}

				/* Action code for transition  */
				 ovenSetPwr(pwr);

				stateVar = Active;/* entry chain  */
				if(stateVarActive== Idle){
					ovenOff();

				}else if(stateVarActive== Cooking){
					ovenOn();

				}else if(stateVarActive== CookingPause){
					
				}else if(stateVarActive== Completed){
					
				}

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
	return evConsumed;
} // end processEvent

