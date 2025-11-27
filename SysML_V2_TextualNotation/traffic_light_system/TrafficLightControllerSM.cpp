/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -d -l cppx -p sysml2text -o system tl.sysml   */
/* This file is generated from tl.sysml - do not edit manually  */
/* Generated on: Thu Nov 27 18:31:04 CET 2025 / 7.0 */


/* Include default headers (no header definition in UML state-chart provided) */
/* Define a header comment in your statediagram.model to define own includes here. */
#include <cstdint>
#include <array>
#include <string>
#include "TrafficLightControllerSM.h"
TrafficLightControllerSM::TrafficLightControllerSM(void)
{
	m_initialized=static_cast<int>(0U);
}



/* State names */
const std::array<std::string, 9> stateNames = { \
	"Operational","OperationalGreen","OperationalRed","OperationalRedYellow","OperationalYellow","OutOfService","OutOfServiceYellowOff","OutOfServiceYellowOn","tlcStateMachine"};



/* Event names */
const std::array<std::string, 9> eventNames = { \
	"evError","evOperational","evTimeout1","evTimeout2","evTimeout3","evTimeout4","evTimeout5","evTimeout6","NO_MSG"};

const std::string TrafficLightControllerSM::getNameByState(const States state) const {
	return (static_cast<const std::string>(stateNames[static_cast<std::uint16_t>(state)]));
}

const std::string TrafficLightControllerSM::getNameByEvent(const TRAFFICLIGHTCONTROLLERSM_EVENT_T evt) const {
	return (static_cast<const std::string>(eventNames[static_cast<std::int16_t>(evt)]));
}


// Helper(s) to reset history
void TrafficLightControllerSM::resetHistorytlcStateMachine(void){
	stateVars.stateVartlcStateMachine=States::OutOfService;
}

void TrafficLightControllerSM::resetHistoryOutOfService(void){
	stateVars.stateVarOutOfService=States::OutOfServiceYellowOn;
}

void TrafficLightControllerSM::resetHistoryOperational(void){
	stateVars.stateVarOperational=States::OperationalRed;
}


bool TrafficLightControllerSM::isInOperational(void) const {return(((stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOperationalGreen(void) const {return(((stateVars.stateVarOperational == States::OperationalGreen) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOperationalRed(void) const {return(((stateVars.stateVarOperational == States::OperationalRed) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOperationalRedYellow(void) const {return(((stateVars.stateVarOperational == States::OperationalRedYellow) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOperationalYellow(void) const {return(((stateVars.stateVarOperational == States::OperationalYellow) && (stateVars.stateVartlcStateMachine == States::Operational) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOutOfService(void) const {return(((stateVars.stateVartlcStateMachine == States::OutOfService) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOutOfServiceYellowOff(void) const {return(((stateVars.stateVarOutOfService == States::OutOfServiceYellowOff) && (stateVars.stateVartlcStateMachine == States::OutOfService) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isInOutOfServiceYellowOn(void) const {return(((stateVars.stateVarOutOfService == States::OutOfServiceYellowOn) && (stateVars.stateVartlcStateMachine == States::OutOfService) && (stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}


bool TrafficLightControllerSM::isIntlcStateMachine(void) const {return(((stateVars.stateVar == States::tlcStateMachine)) ? (true) : (false));}



// Return innermost active states
const std::forward_list<TrafficLightControllerSM::States> TrafficLightControllerSM::getInnermostActiveStates(void) const {
	std::forward_list<TrafficLightControllerSM::States> stateList;

	if ( isInOutOfServiceYellowOn() ) {stateList.push_front(States::OutOfServiceYellowOn);}
	if ( isInOutOfServiceYellowOff() ) {stateList.push_front(States::OutOfServiceYellowOff);}
	if ( isInOperationalRed() ) {stateList.push_front(States::OperationalRed);}
	if ( isInOperationalRedYellow() ) {stateList.push_front(States::OperationalRedYellow);}
	if ( isInOperationalGreen() ) {stateList.push_front(States::OperationalGreen);}
	if ( isInOperationalYellow() ) {stateList.push_front(States::OperationalYellow);}
	return stateList;
}


// Initialize method. Must be called once to init the machine
void TrafficLightControllerSM::initialize(void){
	//call on entry code of default states
	if(m_initialized==static_cast<int>(0U)){
		m_initialized=static_cast<int>(1U);

		//Create copy of statevar
		stateVarsCopy = stateVars;

		// Set state vars to default states

		stateVarsCopy.stateVar = States::tlcStateMachine; /* set init state of top state */
		stateVarsCopy.stateVartlcStateMachine = States::OutOfService; /* set init state of tlcStateMachine */
		stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn; /* set init state of OutOfService */
		stateVarsCopy.stateVarOperational = States::OperationalRed; /* set init state of Operational */




			setYellowLED();
			//Automatically generated from accept after transition
			TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout1Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout1;
			timeoutId1 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout1Message](){onTimeout(evTimeout1Message);});
			

		// Copy state variables back
		stateVars = stateVarsCopy;

	}

}


// State machine event handler
int TrafficLightControllerSM::processEvent(const TRAFFICLIGHTCONTROLLERSM_EVENT_T msg){

	int evConsumed = static_cast<int>(0U);

	

	if(m_initialized==static_cast<int>(0U)){
		return static_cast<int>(0);
	}

	//Create copy of statevar
	stateVarsCopy = stateVars;


	switch (stateVars.stateVar) {

		case States::tlcStateMachine:

			switch (stateVars.stateVartlcStateMachine) {

				case States::OutOfService:

					switch (stateVars.stateVarOutOfService) {

						case States::OutOfServiceYellowOn:
							if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout1){
								/* Transition from OutOfServiceYellowOn to OutOfServiceYellowOff */
								evConsumed = static_cast<int>(1U);
								/* OnExit code of state OutOfServiceYellowOn */
								tm.cancelTimer(timeoutId1);

								/* OnEntry code of state OutOfServiceYellowOff */
								resetYellowLED();
								//Automatically generated from accept after transition
								TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout2Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout2;
								timeoutId2 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout2Message](){onTimeout(evTimeout2Message);});

								/* adjust state variables  */
								stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOff;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						break; /* end of case OutOfServiceYellowOn  */

						case States::OutOfServiceYellowOff:
							if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout2){
								/* Transition from OutOfServiceYellowOff to OutOfServiceYellowOn */
								evConsumed = static_cast<int>(1U);
								/* OnExit code of state OutOfServiceYellowOff */
								tm.cancelTimer(timeoutId2);

								/* OnEntry code of state OutOfServiceYellowOn */
								setYellowLED();
								//Automatically generated from accept after transition
								TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout1Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout1;
								timeoutId1 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout1Message](){onTimeout(evTimeout1Message);});

								/* adjust state variables  */
								stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						break; /* end of case OutOfServiceYellowOff  */

						default:
							/* Intentionally left blank */
						break;
					} /* end switch OutOfService */

					/* Check if event was already processed  */
					if(evConsumed==static_cast<int>(0)){

						if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evOperational){
							/* Transition from OutOfService to Operational */
							evConsumed = static_cast<int>(1U);
							
							if(stateVars.stateVarOutOfService == States::OutOfServiceYellowOn){
								tm.cancelTimer(timeoutId1);


							}else if(stateVars.stateVarOutOfService == States::OutOfServiceYellowOff){
								tm.cancelTimer(timeoutId2);

							}else {
								/* Intentionally left blank */
							};


							setRedLED();
							//Automatically generated from accept after transition
							TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout3Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout3;
							timeoutId3 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(redtime,"SI::second")), [this, evTimeout3Message](){onTimeout(evTimeout3Message);});
							stateVarsCopy.stateVartlcStateMachine = States::Operational; /* Default in entry chain  */
							stateVarsCopy.stateVarOperational = States::OperationalRed; /* Default in entry chain  */

							/* adjust state variables  */
							stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn;
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					}
				break; /* end of case OutOfService  */

				case States::Operational:

					switch (stateVars.stateVarOperational) {

						case States::OperationalRed:
							if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout3){
								/* Transition from OperationalRed to OperationalRedYellow */
								evConsumed = static_cast<int>(1U);
								/* OnExit code of state OperationalRed */
								resetRedLED();
								tm.cancelTimer(timeoutId3);

								/* OnEntry code of state OperationalRedYellow */
								setRedAndYellowLED();
								//Automatically generated from accept after transition
								TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout4Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout4;
								timeoutId4 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(1.0,"SI::second")), [this, evTimeout4Message](){onTimeout(evTimeout4Message);});

								/* adjust state variables  */
								stateVarsCopy.stateVarOperational = States::OperationalRedYellow;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						break; /* end of case OperationalRed  */

						case States::OperationalRedYellow:
							if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout4){
								/* Transition from OperationalRedYellow to OperationalGreen */
								evConsumed = static_cast<int>(1U);
								/* OnExit code of state OperationalRedYellow */
								resetRedAndYellowLED();
								tm.cancelTimer(timeoutId4);

								/* OnEntry code of state OperationalGreen */
								setGreenLED();
								//Automatically generated from accept after transition
								TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout5Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout5;
								timeoutId5 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(1.0,"SI::second")), [this, evTimeout5Message](){onTimeout(evTimeout5Message);});

								/* adjust state variables  */
								stateVarsCopy.stateVarOperational = States::OperationalGreen;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						break; /* end of case OperationalRedYellow  */

						case States::OperationalGreen:
							if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout5){
								/* Transition from OperationalGreen to OperationalYellow */
								evConsumed = static_cast<int>(1U);
								/* OnExit code of state OperationalGreen */
								resetGreenLED();
								tm.cancelTimer(timeoutId5);

								/* OnEntry code of state OperationalYellow */
								setYellowLED();
								//Automatically generated from accept after transition
								TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout6Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout6;
								timeoutId6 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(1.0,"SI::second")), [this, evTimeout6Message](){onTimeout(evTimeout6Message);});

								/* adjust state variables  */
								stateVarsCopy.stateVarOperational = States::OperationalYellow;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						break; /* end of case OperationalGreen  */

						case States::OperationalYellow:
							if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout6){
								/* Transition from OperationalYellow to OperationalRed */
								evConsumed = static_cast<int>(1U);
								/* OnExit code of state OperationalYellow */
								resetYellowLED();
								tm.cancelTimer(timeoutId6);

								/* OnEntry code of state OperationalRed */
								setRedLED();
								//Automatically generated from accept after transition
								TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout3Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout3;
								timeoutId3 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(redtime,"SI::second")), [this, evTimeout3Message](){onTimeout(evTimeout3Message);});

								/* adjust state variables  */
								stateVarsCopy.stateVarOperational = States::OperationalRed;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						break; /* end of case OperationalYellow  */

						default:
							/* Intentionally left blank */
						break;
					} /* end switch Operational */

					/* Check if event was already processed  */
					if(evConsumed==static_cast<int>(0)){

						if(msg==TRAFFICLIGHTCONTROLLERSM_EVENT_T::evError){
							/* Transition from Operational to OutOfService */
							evConsumed = static_cast<int>(1U);
							
							if(stateVars.stateVarOperational == States::OperationalRed){
								resetRedLED();
								tm.cancelTimer(timeoutId3);


							}else if(stateVars.stateVarOperational == States::OperationalRedYellow){
								resetRedAndYellowLED();
								tm.cancelTimer(timeoutId4);


							}else if(stateVars.stateVarOperational == States::OperationalGreen){
								resetGreenLED();
								tm.cancelTimer(timeoutId5);


							}else if(stateVars.stateVarOperational == States::OperationalYellow){
								resetYellowLED();
								tm.cancelTimer(timeoutId6);

							}else {
								/* Intentionally left blank */
							};


							setYellowLED();
							//Automatically generated from accept after transition
							TRAFFICLIGHTCONTROLLERSM_EVENT_T evTimeout1Message = TRAFFICLIGHTCONTROLLERSM_EVENT_T::evTimeout1;
							timeoutId1 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(0.5,"SI::second")), [this, evTimeout1Message](){onTimeout(evTimeout1Message);});
							stateVarsCopy.stateVartlcStateMachine = States::OutOfService; /* Default in entry chain  */
							stateVarsCopy.stateVarOutOfService = States::OutOfServiceYellowOn; /* Default in entry chain  */

							/* adjust state variables  */
							stateVarsCopy.stateVarOperational = States::OperationalRed;
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					}
				break; /* end of case Operational  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch tlcStateMachine */
		break; /* end of case tlcStateMachine  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	// Copy state variables back
	stateVars = stateVarsCopy;

	return (evConsumed);
} // end processEvent






