/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cppx -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Feb 18 13:09:47 CET 2024 / Version 6.3.2.3812 */


#ifndef OVEN_H
#define OVEN_H

// User defined includes
#include <stdint.h>
#include <stdbool.h>

#include <cstdint>
#include <string>
#include <forward_list>
#include "oven_ext.h"
#include "ovenbase.h"
class oven: public ovenbase
{
	public:
		explicit oven();


		auto processEvent(const OVEN_EVENT_T msg) -> std::uint16_t ;

		void initialize();



		enum class States : std::uint32_t {
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

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInInactive() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInActive() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInLightOn() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInLightOff() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInHighPower() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInLowPower() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInCookingPause() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInCooking() const -> bool;

		/** 
		 * Find out if the machine is in a certain state
		 * Returns true if the machine is in this state. False otherwise
		 */ 
		auto isInRadiatorOff() const -> bool;


		/** 
		 * Get information about what state(s) the state machine is in.
		 * Returns a list of one or more states.
		 */ 
		auto getInnermostActiveStates() const -> std::forward_list<oven::States>;


		// Returns the state name as string
		auto getNameByState(const States state) const -> std::string ;

		// Returns the event as string
		auto getNameByEvent(const OVEN_EVENT_T evt) const -> std::string;

	private:

		// Prototype of the validation handler.
		// You have to implement this function yourself and define your own
		// reaction if the validation fails.
		virtual void validationHandler(const States from, const States to) const=0;

		std::uint16_t m_initialized;



	private:
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
		std::uint16_t ovenLight(OVEN_EVENT_T msg);
		std::uint16_t ovenPower(OVEN_EVENT_T msg);
		std::uint16_t ovenRadioator(OVEN_EVENT_T msg);

};

#endif // __OVEN_H__

