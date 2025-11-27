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


#ifndef TRAFFICMANAGEMENTCENTERSM_H
#define TRAFFICMANAGEMENTCENTERSM_H

// User defined includes
#include <stdint.h>
#include <stdbool.h>

#include <system.h>
using namespace TrafficLight;
#include <cstdint>
#include <string>
#include <forward_list>
class TrafficManagementCenterSM: public TrafficManagementCenter
{
	public:
		explicit TrafficManagementCenterSM(void);


		int processEvent(const TRAFFICMANAGEMENTCENTERSM_EVENT_T msg);

		void initialize(void);

		// Helper(s) to reset history
		void resetHistorytmcStateMachine(void);


		enum class States : std::uint16_t {
			Operational,
			PreOperational,
			tmcStateMachine,
			NUM_STATES  // number of states in the machine
		};

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOperational(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInPreOperational(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isIntmcStateMachine(void) const;


		/** 
		 * Get information about what state(s) the state machine is in.
		 * Returns a list of one or more states.
		 */ 
		const std::forward_list<TrafficManagementCenterSM::States> getInnermostActiveStates(void) const;


		// Returns the state name as string
		const std::string getNameByState(const States state) const;

		// Returns the event as string
		const std::string getNameByEvent(const TRAFFICMANAGEMENTCENTERSM_EVENT_T evt) const;

	protected:


		int m_initialized;



	protected:
		// State variables
		struct stateVarsT{
			States stateVar;
			States stateVartmcStateMachine;
		};

		stateVarsT stateVars;
		stateVarsT stateVarsCopy;






};

#endif // __TRAFFICMANAGEMENTCENTERSM_H__

