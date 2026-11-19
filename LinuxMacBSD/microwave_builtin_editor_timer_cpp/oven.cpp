/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2026
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p ssc -l cppx -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sat Nov 14 15:02:19 CET 2026 / 8.0 */


#include "mydefs.h"
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"
#include <stdio.h>
oven::oven(sm::ITimerService& timerService)
	: m_timers(m_timerState, timerService)
{
	m_initialized=static_cast<int>(0U);
}



/* State names */
const char states[]=
	"Completed\0Cooking\0CookingPause\0Idle\0Super\0";

const unsigned short state_idx[]={
	0,10,18,31,36,42};

/* Event names */
const char events[]=
	"evDec\0evDoorClosed\0evDoorOpen\0evInc\0evPwr\0NO_MSG\0";

const unsigned short evt_idx[]={
	0,6,19,30,36,42};

const char* oven::getNameByState(const unsigned short state) const {
	return (states+state_idx[state]);
}

const char* oven::getNameByEvent(const OVEN_EVENT_T evt) const {
	return (events+evt_idx[evt]);
}


// Helper(s) to reset history
void oven::resetHistorySuper(){
	stateVars.stateVarSuper=Idle;
}


bool oven::isInCompleted() const {return(((stateVars.stateVarSuper == Completed) && (stateVars.stateVar == Super)) ? (true) : (false));}


bool oven::isInCooking() const {return(((stateVars.stateVarSuper == Cooking) && (stateVars.stateVar == Super)) ? (true) : (false));}


bool oven::isInCookingPause() const {return(((stateVars.stateVarSuper == CookingPause) && (stateVars.stateVar == Super)) ? (true) : (false));}


bool oven::isInIdle() const {return(((stateVars.stateVarSuper == Idle) && (stateVars.stateVar == Super)) ? (true) : (false));}


bool oven::isInSuper() const {return(((stateVars.stateVar == Super)) ? (true) : (false));}



// Helper to get id of innermost active state
oven::States oven::getInnermostActiveState() const {

	oven::States state = NUM_STATES;

	if(isInIdle()){
		state = Idle;
	}else if(isInCookingPause()){
		state = CookingPause;
	}else if(isInCooking()){
		state = Cooking;
	}else if(isInCompleted()){
		state = Completed;
	}else{
		// intentionally left blank
	}
	return (state);
}

// Initialize method. Must be called once to init the machine
void oven::initialize(){
	//call on entry code of default states
	if(m_initialized==static_cast<int>(0U)){
		m_initialized=static_cast<int>(1U);

		//Create copy of statevar
		stateVarsCopy = stateVars;

		// Set state vars to default states

		stateVarsCopy.stateVar = Super; /* set init state of top state */
		stateVarsCopy.stateVarSuper = Idle; /* set init state of Super */




			ovenOff();
			

		// Copy state variables back
		stateVars = stateVarsCopy;

	}

}


// State machine event handler
int oven::processEvent(const OVEN_MSG_T& msg){

	int evConsumed = 0U;

	

	if(m_initialized==static_cast<int>(0U)){
		return static_cast<int>(0);
	}

	//Create copy of statevar
	stateVarsCopy = stateVars;

	/* action code */
	/* just a comment */


	switch (stateVars.stateVar) {

		case Super:

			switch (stateVars.stateVarSuper) {

				case Completed:
					if(msg.id==evDoorOpen){
						/* Transition from Completed to Idle */
						evConsumed = 1U;

						/* OnEntry code of state Idle */
						ovenOff();

						/* adjust state variables  */
						stateVarsCopy.stateVarSuper = Idle;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Completed  */

				case Cooking:
					if((msg.id==OVEN_NO_MSG)&&(m_timers.expired(TM_Cooking_0))){
						m_timers.consume(TM_Cooking_0, sm::TimerMode::Every); /* every(1 s) taken */
						evConsumed = 16U;
						/* Action code for inner transition  */
						printf(".");
						fflush(stdout);

					}else if((msg.id==OVEN_NO_MSG)&&(m_timers.expired(TM_cookTimer))){
						m_timers.consume(TM_cookTimer, sm::TimerMode::Once); /* cookTimer: timeout taken */
						/* Transition from Cooking to Completed */
						evConsumed = 16U;
						/* OnExit code of state Cooking */
						m_timerTime[TM_cookTimer] = m_timers.timeLeft(TM_cookTimer); /* cookTimer keeps its time left */
						m_timers.stop(TM_cookTimer);
						m_timers.stop(TM_Cooking_0);

						/* Action code for transition  */
						ovenOff();


						/* adjust state variables  */
						stateVarsCopy.stateVarSuper = Completed;
					}else if(msg.id==evDoorOpen){
						/* Transition from Cooking to CookingPause */
						evConsumed = 1U;
						/* OnExit code of state Cooking */
						m_timerTime[TM_cookTimer] = m_timers.timeLeft(TM_cookTimer); /* cookTimer keeps its time left */
						m_timers.stop(TM_cookTimer);
						m_timers.stop(TM_Cooking_0);

						/* Action code for transition  */
						ovenOff();


						/* adjust state variables  */
						stateVarsCopy.stateVarSuper = CookingPause;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Cooking  */

				case CookingPause:
					if(msg.id==evDoorClosed){
						/* Transition from CookingPause to Cooking */
						evConsumed = 1U;

						/* OnEntry code of state Cooking */
						ovenOn();
						m_timers.start(TM_cookTimer, m_timerTime[TM_cookTimer], sm::TimerMode::Once); /* cookTimer: after() */
						m_timers.start(TM_Cooking_0, sm::s<1U>(), sm::TimerMode::Every); /* every(1 s) */

						/* adjust state variables  */
						stateVarsCopy.stateVarSuper = Cooking;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case CookingPause  */

				case Idle:
					if(msg.id==evDoorClosed){
						if((m_timers.runs(TM_cookTimer) ? m_timers.timeLeft(TM_cookTimer) : m_timerTime[TM_cookTimer]) > 0U){
							/* Transition from Idle to Cooking */
							evConsumed = 1U;

							/* OnEntry code of state Cooking */
							ovenOn();
							m_timers.start(TM_cookTimer, m_timerTime[TM_cookTimer], sm::TimerMode::Once); /* cookTimer: after() */
							m_timers.start(TM_Cooking_0, sm::s<1U>(), sm::TimerMode::Every); /* every(1 s) */

							/* adjust state variables  */
							stateVarsCopy.stateVarSuper = Cooking;
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
			if(evConsumed==0){

				if(msg.id==evDec){
					evConsumed = 1U;
					/* Action code for inner transition  */
					/* cookTimer: decrement(1s) */
					{
						const sm::Ticks smTicks = sm::s<1U>();
						if(!m_timers.setTimeLeft(TM_cookTimer, sm::timeSub(m_timers.timeLeft(TM_cookTimer), smTicks))){ /* does not run: the time it keeps */
							m_timerTime[TM_cookTimer] = sm::timeSub(m_timerTime[TM_cookTimer], smTicks);
						}
					}
					ovenDisplay((m_timers.runs(TM_cookTimer) ? m_timers.timeLeft(TM_cookTimer) : m_timerTime[TM_cookTimer]));

				}else if(msg.id==evInc){
					evConsumed = 1U;
					/* Action code for inner transition  */
					/* cookTimer: increment(1s) */
					{
						const sm::Ticks smTicks = sm::s<1U>();
						if(!m_timers.setTimeLeft(TM_cookTimer, sm::timeAdd(m_timers.timeLeft(TM_cookTimer), smTicks))){ /* does not run: the time it keeps */
							m_timerTime[TM_cookTimer] = sm::timeAdd(m_timerTime[TM_cookTimer], smTicks);
						}
					}
					ovenDisplay((m_timers.runs(TM_cookTimer) ? m_timers.timeLeft(TM_cookTimer) : m_timerTime[TM_cookTimer]));

				}else if(msg.id==evPwr){
					const auto pwr = msg.data.evPwr.pwr;
					evConsumed = 1U;
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

	// Copy state variables back
	stateVars = stateVarsCopy;

	return (evConsumed);
} // end processEvent






