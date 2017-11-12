/* (C) Your company*//* Command line options: -Trace -l cppx -v -p ASTAH -o Statemachine -t Demo:SM Model.asta   */
/* This file is generated from Model.asta - do not edit manually  */
/* Generated on: Wed Nov 08 21:44:29 CET 2017 / version 3.7.2 */


#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "Statemachine_ext.h"
class Statemachine
{
	public:
		Statemachine(void);

		int processEvent(STATEMACHINE_EVENT_T msg);

		void initialize();

		// Helper(s) to reset history
		void resetHistorySlow(void);
		void resetHistoryFast(void);

		// Helper(s) to find out if the machine is in a certain state
		bool isInFast(void) const;
		bool isInFastLedOn(void) const;
		bool isInFastLedOff(void) const;
		bool isInOff(void) const;
		bool isInOn(void) const;
		bool isInSlowWaitForLastTimeout(void) const;
		bool isInFastWaitForLastTimeout(void) const;
		bool isInSlow(void) const;
		bool isInSlowLedOn(void) const;
		bool isInSlowLedOff(void) const;


		// Helper returning the innermost active state id.
		unsigned short getInnermostActiveState(void) const;


		// Returns the state name as string
		const char* getNameByState(unsigned short state) const;

		// Returns the event as string
		const char* getNameByEvent(STATEMACHINE_EVENT_T evt) const;

		enum States{
			Fast,
			FastLedOn,
			FastLedOff,
			Off,
			On,
			SlowWaitForLastTimeout,
			FastWaitForLastTimeout,
			Slow,
			SlowLedOn,
			SlowLedOff,
			NUM_STATES  // number of states in the machine
		};

	protected:
		int m_initialized;



	protected:
		// State variables
		struct stateVarsT{
			States stateVar;
			States stateVarSlow;
			States stateVarFast;
		};
		stateVarsT stateVars;
		stateVarsT stateVarsCopy;



		/* Region handler prototypes  */
};

#endif // __STATEMACHINE_H__
