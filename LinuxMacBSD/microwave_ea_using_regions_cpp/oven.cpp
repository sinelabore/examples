/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cppx -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Oct 22 18:06:57 CEST 2023 / Version 6.1.3706 */


#include "oven_ext.h"
#include "oven_hlp.h"
#include "oven.h"
#include <stdio.h>

extern unsigned char msg;
extern T_PWR pwr;
oven::oven(void)
{
	m_initialized=static_cast<int>(0U);
}



/* State names */
const char states[]=
	"Inactive\0Active\0LightOn\0LightOff\0HighPower\0LowPower\0CookingPause\0Cooking\0RadiatorOff\0";

const unsigned short state_idx[]={
	0,9,16,24,33,43,52,65,73,85};

/* Event names */
const char events[]=
	"evPwrLow\0evTimeout\0evDec\0evDoorOpen\0evDoorClosed\0evPwrHigh\0evInc\0NO_MSG\0";

const unsigned short evt_idx[]={
	0,9,19,25,36,49,59,65};

const char* oven::getNameByState(const unsigned short state) const {
	return (states+state_idx[state]);
}

const char* oven::getNameByEvent(const OVEN_EVENT_T evt) const {
	return (events+evt_idx[evt]);
}


// Helper(s) to reset history

// Helper(s) to find out if the machine is in a certain state

bool oven::isInInactive(void) const {return(((stateVars.stateVar== Inactive)) ? (true) : (false));}
bool oven::isInActive(void) const {return(((stateVars.stateVar== Active)) ? (true) : (false));}

// Helper to get id of innermost active state

oven::States oven::getInnermostActiveState(void) const {

	oven::States state = NUM_STATES;

	if(isInActive()){
		state = Active;
	}else if(isInInactive()){
		state = Inactive;
	}else{
		// intentionally left blank
	}
	return (state);
}

// Initialize method. Must be called once to init the machine
void oven::initialize(void){
	//call on entry code of default states
	if(m_initialized==static_cast<int>(0U)){
		m_initialized=static_cast<int>(1U);

		//Create copy of statevar
		stateVarsCopy = stateVars;

		// Set state vars to default states

		stateVarsCopy.stateVar = Inactive; /* set init state of top state */
		stateVarsCopy.stateVarLight = LightOn; /* set init state of Light */
		stateVarsCopy.stateVarPower = LowPower; /* set init state of Power */
		stateVarsCopy.stateVarRadioator = RadiatorOff; /* set init state of Radioator */




			uint8_t testResult = hardwareTest();
			if(testResult==0){
				stateVarsCopy.stateVar = Active;
			switchLight(L_ON);
			
			ovenSetPwr(PWR_LOW);
			
			}else{
				stateVarsCopy.stateVar = Inactive;
			}

		// Copy state variables back
		stateVars = stateVarsCopy;

	}

}


// State machine event handler
int oven::processEvent(const OVEN_EVENT_T msg){

	int evConsumed = 0U;

	

	if(m_initialized==static_cast<int>(0U)){
		return static_cast<int>(0);
	}

	//Create copy of statevar
	stateVarsCopy = stateVars;


	switch (stateVars.stateVar) {

		case Active:
			/* calling region code */
			evConsumed |= ovenLight(msg);
			evConsumed |= ovenPower(msg);
			evConsumed |= ovenRadioator(msg);

			/* Check if event was already processed  */
			if(evConsumed==0){

				if(msg==evDec){
					/* Transition from Active to Active */

					/* Exit code for regions in state Active */
					
					/* Action code for transition  */
					timer_dec();


					/* Entry code for regions in state Active */
					/* entry chain  */
					if(stateVars.stateVarLight == LightOff){
						switchLight(L_OFF);

					}else if(stateVars.stateVarLight == LightOn){
						switchLight(L_ON);

					}else {
						/* Intentionally left blank */
					};

					/* entry chain  */
					if(stateVars.stateVarPower == HighPower){
						ovenSetPwr(PWR_HIGH);

					}else if(stateVars.stateVarPower == LowPower){
						ovenSetPwr(PWR_LOW);

					}else {
						/* Intentionally left blank */
					};

					/* entry chain  */


					/* adjust state variables  */
					stateVarsCopy.stateVar = Active;
				}else if(msg==evInc){
					/* Transition from Active to Active */

					/* Exit code for regions in state Active */
					
					/* Action code for transition  */
					timer_inc();


					/* Entry code for regions in state Active */
					/* entry chain  */
					if(stateVars.stateVarLight == LightOff){
						switchLight(L_OFF);

					}else if(stateVars.stateVarLight == LightOn){
						switchLight(L_ON);

					}else {
						/* Intentionally left blank */
					};

					/* entry chain  */
					if(stateVars.stateVarPower == HighPower){
						ovenSetPwr(PWR_HIGH);

					}else if(stateVars.stateVarPower == LowPower){
						ovenSetPwr(PWR_LOW);

					}else {
						/* Intentionally left blank */
					};

					/* entry chain  */


					/* adjust state variables  */
					stateVarsCopy.stateVar = Active;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Active  */

		case Inactive:
		break; /* end of case Inactive  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	// Copy state variables back
	stateVars = stateVarsCopy;

	return (evConsumed);
} // end processEvent








/* Region code for state Light */

int oven::ovenLight(OVEN_EVENT_T msg){

	int evConsumed = 0U;

	switch (stateVars.stateVarLight) {

		case LightOff:
			if(msg==evDoorClosed){
				if(timer_preset()>0){
					/* Transition from LightOff to LightOn */
					evConsumed=1U;

					/* OnEntry code of state LightOn */
					switchLight(L_ON);

					/* adjust state variables  */
					stateVarsCopy.stateVarLight = LightOn;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LightOff  */

		case LightOn:
			if(msg==evDoorOpen){
				if(timer_preset()==0){
					/* Transition from LightOn to LightOff */
					evConsumed=1U;

					/* OnEntry code of state LightOff */
					switchLight(L_OFF);

					/* adjust state variables  */
					stateVarsCopy.stateVarLight = LightOff;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LightOn  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}




/* Region code for state Power */

int oven::ovenPower(OVEN_EVENT_T msg){

	int evConsumed = 0U;

	switch (stateVars.stateVarPower) {

		case HighPower:
			if(msg==evPwrLow){
				/* Transition from HighPower to LowPower */
				evConsumed=1U;

				/* OnEntry code of state LowPower */
				ovenSetPwr(PWR_LOW);

				/* adjust state variables  */
				stateVarsCopy.stateVarPower = LowPower;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case HighPower  */

		case LowPower:
			if(msg==evPwrHigh){
				/* Transition from LowPower to HighPower */
				evConsumed=1U;

				/* OnEntry code of state HighPower */
				ovenSetPwr(PWR_HIGH);

				/* adjust state variables  */
				stateVarsCopy.stateVarPower = HighPower;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LowPower  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}




/* Region code for state Radioator */

int oven::ovenRadioator(OVEN_EVENT_T msg){

	int evConsumed = 0U;

	switch (stateVars.stateVarRadioator) {

		case Cooking:
			if(msg==evDoorOpen){
				/* Transition from Cooking to CookingPause */
				evConsumed=1U;

				/* Action code for transition  */
				ovenOff();
				timer_pause();


				/* adjust state variables  */
				stateVarsCopy.stateVarRadioator = CookingPause;
			}else if(msg==evTimeout){
				/* Transition from Cooking to RadiatorOff */
				evConsumed=1U;

				/* Action code for transition  */
				ovenOff();
				timer_clear();


				/* adjust state variables  */
				stateVarsCopy.stateVarRadioator = RadiatorOff;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Cooking  */

		case CookingPause:
			if(msg==evDoorClosed){
				/* Transition from CookingPause to Cooking */
				evConsumed=1U;

				/* Action code for transition  */
				timer_cont();


				/* adjust state variables  */
				stateVarsCopy.stateVarRadioator = Cooking;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case CookingPause  */

		case RadiatorOff:
			if(msg==evDoorClosed){
				if(timer_preset()>0){
					/* Transition from RadiatorOff to Cooking */
					evConsumed=1U;

					/* Action code for transition  */
					timer_start();


					/* adjust state variables  */
					stateVarsCopy.stateVarRadioator = Cooking;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case RadiatorOff  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}


