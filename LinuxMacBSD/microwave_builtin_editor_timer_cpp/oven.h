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


#ifndef OVEN_H
#define OVEN_H

// User defined includes
#include <stdint.h>
#include <stdbool.h>

#include "oven_ext.h"
#include "timer_service.hpp" // UseTimerService
class oven
{
	public:
		// Timer ids (UseTimerService): the named timers, then one per timed transition without a name
		enum TimerId : sm::TimerId {
			TM_cookTimer = 0U, // cookTimer: Cooking: after() -> Completed
			TM_Cooking_0 = 1U, // Cooking: every(1 s) internal
			TM_COUNT = 2U,
			// the timer names as the model writes them, for the application's own calls of the service
			cookTimer = TM_cookTimer,
		};

		// Timeouts: the loop hands each timer the service reports as expired to the machine,
		// then calls the machine without event while a timeout is pending
		void setTimeout(sm::TimerId id) { m_timers.setTimeout(id); }
		bool timeoutPending() const { return m_timers.timeoutPending(); }
		// The machine's timers, by which the timer service knows the machine (sm::Timeout::timers)
		sm::MachineTimers& timers() { return m_timers; }

	protected:
		sm::TimerState m_timerState[TM_COUNT]{}; // one per timer: idle, running or expired
		sm::MachineTimers m_timers; // the machine's timers: they work on the states and call the service
		sm::Ticks m_timerTime[1U]{}; // a named timer's time while it does not run

	public:
		explicit oven(sm::ITimerService& timerService);


		int processEvent(const OVEN_MSG_T& msg);

		void initialize();

		// Helper(s) to reset history
		void resetHistorySuper();


		enum States{
			Completed,
			Cooking,
			CookingPause,
			Idle,
			Super,
			NUM_STATES  // number of states in the machine
		};

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInCompleted() const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInCooking() const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInCookingPause() const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInIdle() const;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		bool isInSuper() const;


		// Helper returning the innermost active state id.
		States getInnermostActiveState() const;


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
			States stateVarSuper;
		};

		stateVarsT stateVars{};
		stateVarsT stateVarsCopy{};






};

#endif // __OVEN_H__

