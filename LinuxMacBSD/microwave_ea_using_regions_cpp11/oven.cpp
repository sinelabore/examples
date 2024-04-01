/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -l cppx -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Mon Apr 01 18:02:57 CEST 2024 / Version 6.3.2.3814 */


#include "oven_ext.h"
#include "SafeQueue.hpp"
#include "SimpleTimer.hpp"
#include "oven.h"
#include <stdio.h>

SafeQueue<OVEN_EVENT_T> q;
SimpleTimer<OVEN_EVENT_T> timer(q);

oven::oven()
{
	m_initialized=static_cast<std::uint16_t>(0U);
	initialize();
}



/* State names */
const std::array<std::string, 9> stateNames = { \
	"Inactive","Active","LightOn","LightOff","HighPower","LowPower","CookingPause","Cooking","RadiatorOff"};



/* Event names */
const std::array<std::string, 8> eventNames = { \
	"evPwrLow","evTimeout","evDec","evDoorOpen","evDoorClosed","evPwrHigh","evInc","NO_MSG"};

auto oven::getNameByState(const States state) const -> std::string {
	return (static_cast<const std::string>(stateNames[static_cast<std::uint32_t>(state)]));
}

auto oven::getNameByEvent(const OVEN_EVENT_T evt) const -> std::string {
	return (static_cast<const std::string>(eventNames[static_cast<std::uint16_t>(evt)]));
}



auto oven::isInInactive() const -> bool {return(((stateVars.stateVar == States::Inactive)) ? (true) : (false));}


auto oven::isInActive() const -> bool {return(((stateVars.stateVar == States::Active)) ? (true) : (false));}


auto oven::isInLightOn() const -> bool {return(((stateVars.stateVarLight == States::LightOn) && isInActive()));}


auto oven::isInLightOff() const -> bool {return(((stateVars.stateVarLight == States::LightOff) && isInActive()));}


auto oven::isInHighPower() const -> bool {return(((stateVars.stateVarPower == States::HighPower) && isInActive()));}


auto oven::isInLowPower() const -> bool {return(((stateVars.stateVarPower == States::LowPower) && isInActive()));}


auto oven::isInCookingPause() const -> bool {return(((stateVars.stateVarRadioator == States::CookingPause) && isInActive()));}


auto oven::isInCooking() const -> bool {return(((stateVars.stateVarRadioator == States::Cooking) && isInActive()));}


auto oven::isInRadiatorOff() const -> bool {return(((stateVars.stateVarRadioator == States::RadiatorOff) && isInActive()));}



// Return a list with the states in which the state machine is currently in 
auto oven::getInnermostActiveStates() const -> std::forward_list<oven::States> {
	std::forward_list<oven::States> stateList;

	if ( isInLightOff() ) {stateList.push_front(States::LightOff);}
	if ( isInLightOn() ) {stateList.push_front(States::LightOn);}
	if ( isInHighPower() ) {stateList.push_front(States::HighPower);}
	if ( isInLowPower() ) {stateList.push_front(States::LowPower);}
	if ( isInCooking() ) {stateList.push_front(States::Cooking);}
	if ( isInCookingPause() ) {stateList.push_front(States::CookingPause);}
	if ( isInRadiatorOff() ) {stateList.push_front(States::RadiatorOff);}
	if ( isInInactive() ) {stateList.push_front(States::Inactive);}
	return stateList;
}


// Initialize method. Must be called once to init the machine
void oven::initialize(){
	//call on entry code of default states
	if(m_initialized==static_cast<std::uint16_t>(0U)){
		m_initialized=static_cast<std::uint16_t>(1U);

		//Create copy of statevar
		stateVarsCopy = stateVars;

		// Set state vars to default states

		stateVarsCopy.stateVar = States::Inactive; /* set init state of top state */
		stateVarsCopy.stateVarLight = States::LightOff; /* set init state of Light */
		stateVarsCopy.stateVarPower = States::LowPower; /* set init state of Power */
		stateVarsCopy.stateVarRadioator = States::RadiatorOff; /* set init state of Radioator */




			timer.create(OVEN_EVENT_T::evTimeout);
			uint8_t testResult = hardwareTest();
			if(testResult==0){
				stateVarsCopy.stateVar = States::Active;
			}else{
				stateVarsCopy.stateVar = States::Inactive;
			}

		// Copy state variables back
		stateVars = stateVarsCopy;

	}

}


// State machine event handler
auto oven::processEvent(const OVEN_EVENT_T msg) -> std::uint16_t {

	auto evConsumed = static_cast<std::uint16_t>(0U);

	

	//Create copy of statevar
	stateVarsCopy = stateVars;


	switch (stateVars.stateVar) {

		case States::Active:
			/* calling region code  */
			evConsumed |= ovenLight(msg);
			evConsumed |= ovenPower(msg);
			evConsumed |= ovenRadioator(msg);

			/* Check if event was already processed  */
			if(evConsumed==static_cast<std::uint16_t>(0)){

				if(msg==OVEN_EVENT_T::evDec){
					/* Transition from Active to Active */
					validationHandler(States::Active, States::Active);

					/* Exit code for regions in state Active */
					
					/* Action code for transition  */
					timer.dec();


					/* Entry code for regions in state Active */
					/* entry chain  */
					/* entry chain  */
					/* entry chain  */


					/* adjust state variables  */
					stateVarsCopy.stateVar = States::Active;
				}else if(msg==OVEN_EVENT_T::evInc){
					/* Transition from Active to Active */
					validationHandler(States::Active, States::Active);

					/* Exit code for regions in state Active */
					
					/* Action code for transition  */
					timer.inc();


					/* Entry code for regions in state Active */
					/* entry chain  */
					/* entry chain  */
					/* entry chain  */


					/* adjust state variables  */
					stateVarsCopy.stateVar = States::Active;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case Active  */

		case States::Inactive:
		break; /* end of case Inactive  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	// Copy state variables back
	stateVars = stateVarsCopy;

	return (evConsumed);
} // end processEvent








/* Region code for state Light */

std::uint16_t oven::ovenLight(OVEN_EVENT_T msg){

	std::uint16_t evConsumed = 0U;

	switch (stateVars.stateVarLight) {

		case States::LightOff:
			if(msg==OVEN_EVENT_T::evDoorClosed){
				if(timer.preset()>0){
					/* Transition from LightOff to LightOn */
					validationHandler(States::LightOff, States::LightOn);
					evConsumed=static_cast<std::uint16_t>(1U);

					/* Action code for transition  */
					switchLight(T_LIGHT::L_ON);


					/* adjust state variables  */
					stateVarsCopy.stateVarLight = States::LightOn;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LightOff  */

		case States::LightOn:
			if(msg==OVEN_EVENT_T::evDoorOpen){
				if(timer.preset()==0){
					/* Transition from LightOn to LightOff */
					validationHandler(States::LightOn, States::LightOff);
					evConsumed=static_cast<std::uint16_t>(1U);

					/* Action code for transition  */
					switchLight(T_LIGHT::L_OFF);


					/* adjust state variables  */
					stateVarsCopy.stateVarLight = States::LightOff;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LightOn  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}




/* Region code for state Power */

std::uint16_t oven::ovenPower(OVEN_EVENT_T msg){

	std::uint16_t evConsumed = 0U;

	switch (stateVars.stateVarPower) {

		case States::HighPower:
			if(msg==OVEN_EVENT_T::evPwrLow){
				/* Transition from HighPower to LowPower */
				validationHandler(States::HighPower, States::LowPower);
				evConsumed=static_cast<std::uint16_t>(1U);

				/* Action code for transition  */
				ovenSetPwr(T_PWR::PWR_LOW);


				/* adjust state variables  */
				stateVarsCopy.stateVarPower = States::LowPower;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case HighPower  */

		case States::LowPower:
			if(msg==OVEN_EVENT_T::evPwrHigh){
				/* Transition from LowPower to HighPower */
				validationHandler(States::LowPower, States::HighPower);
				evConsumed=static_cast<std::uint16_t>(1U);

				/* Action code for transition  */
				ovenSetPwr(T_PWR::PWR_HIGH);


				/* adjust state variables  */
				stateVarsCopy.stateVarPower = States::HighPower;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case LowPower  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}




/* Region code for state Radioator */

std::uint16_t oven::ovenRadioator(OVEN_EVENT_T msg){

	std::uint16_t evConsumed = 0U;

	switch (stateVars.stateVarRadioator) {

		case States::Cooking:
			if(msg==OVEN_EVENT_T::evDoorOpen){
				/* Transition from Cooking to CookingPause */
				validationHandler(States::Cooking, States::CookingPause);
				evConsumed=static_cast<std::uint16_t>(1U);

				/* Action code for transition  */
				ovenOff();  timer.pause();


				/* adjust state variables  */
				stateVarsCopy.stateVarRadioator = States::CookingPause;
			}else if(msg==OVEN_EVENT_T::evTimeout){
				/* Transition from Cooking to RadiatorOff */
				validationHandler(States::Cooking, States::RadiatorOff);
				evConsumed=static_cast<std::uint16_t>(1U);

				/* Action code for transition  */
				ovenOff();


				/* adjust state variables  */
				stateVarsCopy.stateVarRadioator = States::RadiatorOff;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case Cooking  */

		case States::CookingPause:
			if(msg==OVEN_EVENT_T::evDoorClosed){
				/* Transition from CookingPause to Cooking */
				validationHandler(States::CookingPause, States::Cooking);
				evConsumed=static_cast<std::uint16_t>(1U);

				/* Action code for transition  */
				timer.cont();
				ovenOn();


				/* adjust state variables  */
				stateVarsCopy.stateVarRadioator = States::Cooking;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case CookingPause  */

		case States::RadiatorOff:
			if(msg==OVEN_EVENT_T::evDoorClosed){
				if(timer.preset()>0){
					/* Transition from RadiatorOff to Cooking */
					validationHandler(States::RadiatorOff, States::Cooking);
					evConsumed=static_cast<std::uint16_t>(1U);

					/* Action code for transition  */
					timer.start(SimpleTimerKind::SINGLE,timer.preset());
					ovenOn();


					/* adjust state variables  */
					stateVarsCopy.stateVarRadioator = States::Cooking;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case RadiatorOff  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */

	return evConsumed;
}


