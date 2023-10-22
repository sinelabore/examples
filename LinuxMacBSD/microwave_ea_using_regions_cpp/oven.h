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


#ifndef OVEN_H
#define OVEN_H

// User defined includes
#include <stdint.h>
#include <stdbool.h>

#include "oven_ext.h"
#include "ovenbase.h"
class oven: public ovenbase
{
	public:
		explicit oven(void);


		int processEvent(const OVEN_EVENT_T msg);

		void initialize(void);


		// Helper(s) to find out if the machine is in a certain state
		bool isInInactive(void) const;
		bool isInActive(void) const;


		// Helper returning the innermost active state id.
		enum States{
			Inactive,
			Active,
			LightOn,
			LightOff,
			HighPower,
			LowPower,
			CookingPause,
			Cooking,
			RadiatorOff,
			NUM_STATES  // number of states in the machine
		};

		States getInnermostActiveState(void) const;


		// Returns the state name as string
		const char* getNameByState(const unsigned short state) const;

		// Returns the event as string
		const char* getNameByEvent(const OVEN_EVENT_T evt) const;

	protected:


		int m_initialized;



	protected:
		// State variables
		struct stateVarsT{
			States stateVar;
			States stateVarLight;
			States stateVarPower;
			States stateVarRadioator;
		};

		stateVarsT stateVars;
		stateVarsT stateVarsCopy;





		/* Region handler prototypes  */
		int ovenLight(OVEN_EVENT_T msg);
		int ovenPower(OVEN_EVENT_T msg);
		int ovenRadioator(OVEN_EVENT_T msg);

};

#endif // __OVEN_H__

