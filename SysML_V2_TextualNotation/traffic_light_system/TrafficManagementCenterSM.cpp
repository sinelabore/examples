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
#include "TrafficManagementCenterSM.h"
TrafficManagementCenterSM::TrafficManagementCenterSM(void)
{
	m_initialized=static_cast<int>(0U);
}



/* State names */
const std::array<std::string, 3> stateNames = { \
	"Operational","PreOperational","tmcStateMachine"};



/* Event names */
const std::array<std::string, 2> eventNames = { \
	"evTimeout0","NO_MSG"};

const std::string TrafficManagementCenterSM::getNameByState(const States state) const {
	return (static_cast<const std::string>(stateNames[static_cast<std::uint16_t>(state)]));
}

const std::string TrafficManagementCenterSM::getNameByEvent(const TRAFFICMANAGEMENTCENTERSM_EVENT_T evt) const {
	return (static_cast<const std::string>(eventNames[static_cast<std::int16_t>(evt)]));
}


// Helper(s) to reset history
void TrafficManagementCenterSM::resetHistorytmcStateMachine(void){
	stateVars.stateVartmcStateMachine=States::PreOperational;
}


bool TrafficManagementCenterSM::isInOperational(void) const {return(((stateVars.stateVartmcStateMachine == States::Operational) && (stateVars.stateVar == States::tmcStateMachine)) ? (true) : (false));}


bool TrafficManagementCenterSM::isInPreOperational(void) const {return(((stateVars.stateVartmcStateMachine == States::PreOperational) && (stateVars.stateVar == States::tmcStateMachine)) ? (true) : (false));}


bool TrafficManagementCenterSM::isIntmcStateMachine(void) const {return(((stateVars.stateVar == States::tmcStateMachine)) ? (true) : (false));}



// Return innermost active states
const std::forward_list<TrafficManagementCenterSM::States> TrafficManagementCenterSM::getInnermostActiveStates(void) const {
	std::forward_list<TrafficManagementCenterSM::States> stateList;

	if ( isInPreOperational() ) {stateList.push_front(States::PreOperational);}
	if ( isInOperational() ) {stateList.push_front(States::Operational);}
	return stateList;
}


// Initialize method. Must be called once to init the machine
void TrafficManagementCenterSM::initialize(void){
	//call on entry code of default states
	if(m_initialized==static_cast<int>(0U)){
		m_initialized=static_cast<int>(1U);

		//Create copy of statevar
		stateVarsCopy = stateVars;

		// Set state vars to default states

		stateVarsCopy.stateVar = States::tmcStateMachine; /* set init state of top state */
		stateVarsCopy.stateVartmcStateMachine = States::PreOperational; /* set init state of tmcStateMachine */




			//Automatically generated from accept after transition
			TRAFFICMANAGEMENTCENTERSM_EVENT_T evTimeout0Message = TRAFFICMANAGEMENTCENTERSM_EVENT_T::evTimeout0;
			timeoutId0 = tm.startTimer(std::chrono::milliseconds(msFromTimeUnit(2.0,"SI::second")), [this, evTimeout0Message](){onTimeout(evTimeout0Message);});
			

		// Copy state variables back
		stateVars = stateVarsCopy;

	}

}


// State machine event handler
int TrafficManagementCenterSM::processEvent(const TRAFFICMANAGEMENTCENTERSM_EVENT_T msg){

	int evConsumed = static_cast<int>(0U);

	

	if(m_initialized==static_cast<int>(0U)){
		return static_cast<int>(0);
	}

	//Create copy of statevar
	stateVarsCopy = stateVars;


	switch (stateVars.stateVar) {

		case States::tmcStateMachine:

			switch (stateVars.stateVartmcStateMachine) {

				case States::PreOperational:
					if(msg==TRAFFICMANAGEMENTCENTERSM_EVENT_T::evTimeout0){
						/* Transition from PreOperational to Operational */
						evConsumed = static_cast<int>(1U);
						/* OnExit code of state PreOperational */
						tm.cancelTimer(timeoutId0);

						/* Action code for transition  */
						sendPort.send(ControlPortDataDef{TRAFFICLIGHTCONTROLLERSM_EVENT_T::evOperational});


						/* adjust state variables  */
						stateVarsCopy.stateVartmcStateMachine = States::Operational;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case PreOperational  */

				case States::Operational:
				break; /* end of case Operational  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch tmcStateMachine */
		break; /* end of case tmcStateMachine  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	// Copy state variables back
	stateVars = stateVarsCopy;

	return (evConsumed);
} // end processEvent






