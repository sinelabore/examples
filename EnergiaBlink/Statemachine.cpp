/* (C) Your company*//* Command line options: -l cppx -v -p ASTAH -o Statemachine -t Demo:SM Model.asta   */
/* This file is generated from Model.asta - do not edit manually  */
/* Generated on: Sat Sep 16 09:57:54 CEST 2017 / version 3.7 */


#include <Energia.h>
#include <stdio.h>
#include <stdint.h>
#include "Statemachine_ext.h"
#include "Statemachine.h"

extern "C"
{
#include "timer.h"
#include "fifo.h"
}


extern uint8_t timerId;
extern FIFO_T fifo; // fifo object

Statemachine::Statemachine(void)
{
	m_initialized=0U;
}



/* State names */
const char states[]=
	"Fast\0FastLedOn\0FastLedOff\0Off\0On\0SlowWaitForLastTimeout\0FastWaitForLastTimeout\0Slow\0SlowLedOn\0SlowLedOff\0";

const unsigned short state_idx[]={
	0,5,15,26,30,33,56,79,84,94,105};

/* Event names */
const char events[]=
	"evTimeout\0evButton2\0evButton1\0NO_MSG\0";

const unsigned short evt_idx[]={
	0,10,20,30};

const char* Statemachine::getNameByState(unsigned short state) const {
	return states+state_idx[state];
}

const char* Statemachine::getNameByEvent(STATEMACHINE_EVENT_T evt) const {
	return events+evt_idx[evt];
}


// Helper(s) to reset history
void Statemachine::resetHistorySlow(void){
	stateVars.stateVarSlow=SlowLedOn;
}

void Statemachine::resetHistoryFast(void){
	stateVars.stateVarFast=FastLedOn;
}


// Helper(s) to find out if the machine is in a certain state

bool Statemachine::isInFast(void) const {return(((stateVars.stateVar== Fast)) ? (true) : (false));}
bool Statemachine::isInFastLedOn(void) const {return(((stateVars.stateVarFast== FastLedOn)&&(stateVars.stateVar== Fast)) ? (true) : (false));}
bool Statemachine::isInFastLedOff(void) const {return(((stateVars.stateVarFast== FastLedOff)&&(stateVars.stateVar== Fast)) ? (true) : (false));}
bool Statemachine::isInOff(void) const {return(((stateVars.stateVar== Off)) ? (true) : (false));}
bool Statemachine::isInOn(void) const {return(((stateVars.stateVar== On)) ? (true) : (false));}
bool Statemachine::isInSlowWaitForLastTimeout(void) const {return(((stateVars.stateVar== SlowWaitForLastTimeout)) ? (true) : (false));}
bool Statemachine::isInFastWaitForLastTimeout(void) const {return(((stateVars.stateVar== FastWaitForLastTimeout)) ? (true) : (false));}
bool Statemachine::isInSlow(void) const {return(((stateVars.stateVar== Slow)) ? (true) : (false));}
bool Statemachine::isInSlowLedOn(void) const {return(((stateVars.stateVarSlow== SlowLedOn)&&(stateVars.stateVar== Slow)) ? (true) : (false));}
bool Statemachine::isInSlowLedOff(void) const {return(((stateVars.stateVarSlow== SlowLedOff)&&(stateVars.stateVar== Slow)) ? (true) : (false));}

// Helper to get id of innermost active state

unsigned short Statemachine::getInnermostActiveState(void) const {
	if(isInSlowLedOff()){
		return SlowLedOff;
	}else if(isInSlowLedOn()){
		return SlowLedOn;
	}else if(isInFastLedOff()){
		return FastLedOff;
	}else if(isInFastLedOn()){
		return FastLedOn;
	}else if(isInFastWaitForLastTimeout()){
		return FastWaitForLastTimeout;
	}else if(isInSlowWaitForLastTimeout()){
		return SlowWaitForLastTimeout;
	}else if(isInOn()){
		return On;
	}else if(isInOff()){
		return Off;
	}else{
		return NUM_STATES;
	}
}
// Initialize method. Must be called once to init the machine
void Statemachine::initialize(){
	//call on entry code of default states
	if(m_initialized==0U){
		m_initialized=1U;

		//Create copy of statevar
		stateVarsCopy = stateVars;

		// Set state vars to default states

		stateVarsCopy.stateVar = Slow; /* set init state of top state */
		stateVarsCopy.stateVarSlow = SlowLedOn; /* set init state of Slow */
		stateVarsCopy.stateVarFast = FastLedOn; /* set init state of Fast */

		// Copy state variables back
		stateVars = stateVarsCopy;

	}



	 timerId = timerCreate(&fifo, evTimeout, 1);
	timerStart(timerId,T1S);
	
	digitalWrite(RED_LED, HIGH);
	

}

// State machine event handler
int Statemachine::processEvent(STATEMACHINE_EVENT_T msg){

	int evConsumed = 0U;

	

	if(m_initialized==0U) return 0;

	//Create copy of statevar
	stateVarsCopy = stateVars;


	switch (stateVars.stateVar) {

		case Slow:

			switch (stateVars.stateVarSlow) {

				case SlowLedOn:
					if(msg==evTimeout){
						/* Transition from SlowLedOn to SlowLedOff */
						evConsumed=1;

						/* OnEntry code of state SlowLedOff */
						digitalWrite(RED_LED, LOW);

						/* adjust state variables  */
						stateVarsCopy.stateVarSlow = SlowLedOff;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case SlowLedOn  */

				case SlowLedOff:
					if(msg==evTimeout){
						/* Transition from SlowLedOff to SlowLedOn */
						evConsumed=1;

						/* OnEntry code of state SlowLedOn */
						digitalWrite(RED_LED, HIGH);

						/* adjust state variables  */
						stateVarsCopy.stateVarSlow = SlowLedOn;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case SlowLedOff  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Slow */

			/* Check if event was already processed  */
			if(evConsumed==0){

				if(msg==evButton1){
					/* Transition from Slow to SlowWaitForLastTimeout */
					evConsumed=1;
					

					/* adjust state variables  */
					stateVarsCopy.stateVar = SlowWaitForLastTimeout;
				}else if(msg==evButton2){
					/* Transition from Slow to Fast */
					evConsumed=1;
					
					/* Action code for transition  */
					timerStop(timerId);

					/* OnEntry code of state Fast */
					timerStart(timerId,T100MS);
					digitalWrite(RED_LED, HIGH);
					stateVarsCopy.stateVar = Fast;/* Default in entry chain  */
					stateVarsCopy.stateVarFast = FastLedOn;/* Default in entry chain  */

				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Slow  */

		case Fast:

			switch (stateVars.stateVarFast) {

				case FastLedOn:
					if(msg==evTimeout){
						/* Transition from FastLedOn to FastLedOff */
						evConsumed=1;

						/* OnEntry code of state FastLedOff */
						digitalWrite(RED_LED, LOW);

						/* adjust state variables  */
						stateVarsCopy.stateVarFast = FastLedOff;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case FastLedOn  */

				case FastLedOff:
					if(msg==evTimeout){
						/* Transition from FastLedOff to FastLedOn */
						evConsumed=1;

						/* OnEntry code of state FastLedOn */
						digitalWrite(RED_LED, HIGH);

						/* adjust state variables  */
						stateVarsCopy.stateVarFast = FastLedOn;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case FastLedOff  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch Fast */

			/* Check if event was already processed  */
			if(evConsumed==0){

				if(msg==evButton1){
					/* Transition from Fast to FastWaitForLastTimeout */
					evConsumed=1;
					

					/* adjust state variables  */
					stateVarsCopy.stateVar = FastWaitForLastTimeout;
				}else if(msg==evButton2){
					/* Transition from Fast to Slow */
					evConsumed=1;
					
					/* Action code for transition  */
					timerStop(timerId);


					/* OnEntry code of state Slow */
					timerStart(timerId,T1S);
					digitalWrite(RED_LED, HIGH);
					stateVarsCopy.stateVar = Slow;/* Default in entry chain  */
					stateVarsCopy.stateVarSlow = SlowLedOn;/* Default in entry chain  */

				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Fast  */

		case Off:
			if(msg==evButton1){
				/* Transition from Off to Slow */
				evConsumed=1;

				/* OnEntry code of state Slow */
				timerStart(timerId,T1S);
				digitalWrite(RED_LED, HIGH);
				stateVarsCopy.stateVar = Slow;/* Default in entry chain  */
				stateVarsCopy.stateVarSlow = SlowLedOn;/* Default in entry chain  */

			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Off  */

		case On:
			if(msg==evButton1){
				/* Transition from On to Fast */
				evConsumed=1;

				/* OnEntry code of state Fast */
				timerStart(timerId,T100MS);
				digitalWrite(RED_LED, HIGH);
				stateVarsCopy.stateVar = Fast;/* Default in entry chain  */
				stateVarsCopy.stateVarFast = FastLedOn;/* Default in entry chain  */

			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case On  */

		case SlowWaitForLastTimeout:
			if(msg==evTimeout){
				/* Transition from SlowWaitForLastTimeout to Off */
				evConsumed=1;

				/* OnEntry code of state Off */
				timerStop(timerId);
				digitalWrite(RED_LED, LOW);

				/* adjust state variables  */
				stateVarsCopy.stateVar = Off;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case SlowWaitForLastTimeout  */

		case FastWaitForLastTimeout:
			if(msg==evTimeout){
				/* Transition from FastWaitForLastTimeout to On */
				evConsumed=1;

				/* OnEntry code of state On */
				timerStop(timerId);
				digitalWrite(RED_LED, HIGH);

				/* adjust state variables  */
				stateVarsCopy.stateVar = On;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case FastWaitForLastTimeout  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
	// Copy state variables back
	stateVars = stateVarsCopy;

	return evConsumed;
} // end processEvent






