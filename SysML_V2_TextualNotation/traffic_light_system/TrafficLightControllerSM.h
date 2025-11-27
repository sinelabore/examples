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


#ifndef TRAFFICLIGHTCONTROLLERSM_H
#define TRAFFICLIGHTCONTROLLERSM_H

// User defined includes
#include <stdint.h>
#include <stdbool.h>

#include <system.h>
using namespace TrafficLight;
#include <cstdint>
#include <string>
#include <forward_list>
class TrafficLightControllerSM: public TrafficLightController
{
	public:
		explicit TrafficLightControllerSM(void);


		int processEvent(const TRAFFICLIGHTCONTROLLERSM_EVENT_T msg);

		void initialize(void);

		// Helper(s) to reset history
		void resetHistorytlcStateMachine(void);
		void resetHistoryOutOfService(void);
		void resetHistoryOperational(void);


		enum class States : std::uint16_t {
			Operational,
			OperationalGreen,
			OperationalRed,
			OperationalRedYellow,
			OperationalYellow,
			OutOfService,
			OutOfServiceYellowOff,
			OutOfServiceYellowOn,
			tlcStateMachine,
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
		bool isInOperationalGreen(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOperationalRed(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOperationalRedYellow(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOperationalYellow(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOutOfService(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOutOfServiceYellowOff(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInOutOfServiceYellowOn(void) const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isIntlcStateMachine(void) const;


		/** 
		 * Get information about what state(s) the state machine is in.
		 * Returns a list of one or more states.
		 */ 
		const std::forward_list<TrafficLightControllerSM::States> getInnermostActiveStates(void) const;


		// Returns the state name as string
		const std::string getNameByState(const States state) const;

		// Returns the event as string
		const std::string getNameByEvent(const TRAFFICLIGHTCONTROLLERSM_EVENT_T evt) const;

	protected:


		int m_initialized;



	protected:
		// State variables
		struct stateVarsT{
			States stateVar;
			States stateVartlcStateMachine;
			States stateVarOutOfService;
			States stateVarOperational;
		};

		stateVarsT stateVars;
		stateVarsT stateVarsCopy;






};

#endif // __TRAFFICLIGHTCONTROLLERSM_H__

