/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p ssc -l mjs oven.xml  */
/* This file is generated from oven.xml - do not edit manually */
/* Generated on: Mon Jan 22 21:11:59 CET 2024 / Version 6.3.3788*/



import { TimerManager } from './TimerManager.mjs';
import { DisplayInterface } from './DisplayInterface.mjs';


export function oven() {

	this.init =  false;
	this.instId = 0;
	// State machine state data
	this.stateVars = {
		stateVar: null,
		stateVarMainRegion : null,
		stateVarCookingRegion : null,
	};

	// States of the state machine 
	this.states = Object.freeze({
			none: 0,
			Super: 1,
			Completed: 2,
			Cooking: 3,
			CookingPause: 4,
			Idle: 5,
			TimeNotShown: 6,
			TimeShown: 7,
		})

	// Events the state machine can process
	this.events = Object.freeze({
			none: 0,
			evTimeout: 1,
			evDec: 2,
			evDoorOpen: 3,
			evDoorClosed: 4,
			evInc: 5,
			evBlink: 6,
		})


	// Initialize state machine
	this.initialize = function(){
		this.init = true;
		// Set state vars to default states
		this.stateVars.stateVar = this.states.Super /* set init state of top state */
		this.stateVars.stateVarMainRegion = this.states.Idle; /* set init state of MainRegion */
		this.stateVars.stateVarCookingRegion = this.states.TimeShown; /* set init state of CookingRegion */

		this.tid = document.timerMgr.createTimer(0, this.events.evTimeout);
		this.tidBlink = document.timerMgr.createTimer(500, this.events.evBlink);
		this.display = new DisplayInterface();

	}

	this.isInSuper = function() { return (this.stateVars.stateVar === this.states.Super);}
	this.isInTimeNotShown = function() { return (this.stateVars.stateVarCookingRegion === this.states.TimeNotShown) && this.isInCooking();}
	this.isInTimeShown = function() { return (this.stateVars.stateVarCookingRegion === this.states.TimeShown) && this.isInCooking();}
	this.isInCompleted = function() { return (this.stateVars.stateVarMainRegion === this.states.Completed) && this.isInSuper();}
	this.isInCooking = function() { return (this.stateVars.stateVarMainRegion === this.states.Cooking) && this.isInSuper();}
	this.isInCookingPause = function() { return (this.stateVars.stateVarMainRegion === this.states.CookingPause) && this.isInSuper();}
	this.isInIdle = function() { return (this.stateVars.stateVarMainRegion === this.states.Idle) && this.isInSuper();}

	//Return the state name string based on the state value
	this.stateToString = function(stateValue) {
		const value = Object.keys(this.states).find(key => this.states[key] === stateValue);
		return value || `UnknownState(${stateValue})`;
	}

	//Return the event name string based on the event value
	this.eventToString = function(eventValue) {
		const value = Object.keys(this.events).find(key => this.events[key] === eventValue);
		return value || `UnknownEvent(${eventValue})`;
	}


	// Return of a map with the states in which the state machine is currently in
	this.innermostActiveStates = function() {
		var statesMap = new Map();
		if ( this.isInCompleted() ) {statesMap.set(this.states.Completed,"Completed");}
		if ( this.isInTimeShown() ) {statesMap.set(this.states.TimeShown,"TimeShown");}
		if ( this.isInTimeNotShown() ) {statesMap.set(this.states.TimeNotShown,"TimeNotShown");}
		if ( this.isInCookingPause() ) {statesMap.set(this.states.CookingPause,"CookingPause");}
		if ( this.isInIdle() ) {statesMap.set(this.states.Idle,"Idle");}
		return statesMap;
	}


	// State machine event handler
	this.processEvent = function(msg, userData) {
		var evConsumed=0;
		if (this.init === false) {
			this.initialize()
		}
		
		// Copy stateVar to ensure regions have same view on machine
		this.stateVarsCopy = Object.assign({}, this.stateVars);		
			// Action code 
			/* Action - sample */
			console.log(this.eventToString(msg));



		switch (this.stateVars.stateVar) {

			case this.states.Super:
				/* calling region code */
				evConsumed |= this.processEventMainRegion(msg, userData);

				/* Check if event was already processed */
				if(evConsumed===0){

					if (msg === this.events.evDec) {
						/* Transition from Super to Super*/

						/* Exit code for regions in state Super*/
						
						if(this.stateVars.stateVarMainRegion == this.states.Cooking){
							
						}else {
							/* Intentionally left blank */
						};


						/* Action code for transition */
						document.timerMgr.decTimer(this.tid);
						this.display.setTT(document.timerMgr.getPresetInSec(this.tid));


						/* Entry code for regions in state Super*/
						/* entry chain */
						if(this.stateVars.stateVarMainRegion == this.states.Cooking){
							
							/* entry chain */
						}else {
							/* Intentionally left blank */
						};



						/* adjust state variables */
						this.stateVars.stateVar = this.states.Super;
					} else if (msg === this.events.evInc) {
						/* Transition from Super to Super*/

						/* Exit code for regions in state Super*/
						
						if(this.stateVars.stateVarMainRegion == this.states.Cooking){
							
						}else {
							/* Intentionally left blank */
						};


						/* Action code for transition */
						document.timerMgr.incTimer(this.tid);
						this.display.setTT(document.timerMgr.getPresetInSec(this.tid));


						/* Entry code for regions in state Super*/
						/* entry chain */
						if(this.stateVars.stateVarMainRegion == this.states.Cooking){
							
							/* entry chain */
						}else {
							/* Intentionally left blank */
						};



						/* adjust state variables */
						this.stateVars.stateVar = this.states.Super;
					} else {
						/* Intentionally left blank*/
					} /*end of event selection*/
				}
			break; /* end of case Super */

			default:
				/* Intentionally left blank*/
			break;
		} /* end switch stateVar_root*/

	// Post Action Code
		/* Post-Action - sample */

		
		this.stateVars = Object.assign({}, this.stateVarsCopy);		
		return evConsumed
	}



	// Region code for region CookingRegion
	this.processEventCookingRegion = function(msg, userData) {
		var evConsumed = 0;


		switch (this.stateVars.stateVarCookingRegion) {

			case this.states.TimeShown:
				if (msg === this.events.evBlink) {
					/* Transition from TimeShown to TimeNotShown*/
					evConsumed=1;

					/* Action code for transition */
					this.display.setTT("");


					/* adjust state variables */
					this.stateVarsCopy.stateVarCookingRegion = this.states.TimeNotShown;
				} else {
					/* Intentionally left blank*/
				} /*end of event selection*/
			break; /* end of case TimeShown */

			case this.states.TimeNotShown:
				if (msg === this.events.evBlink) {
					/* Transition from TimeNotShown to TimeShown*/
					evConsumed=1;

					/* Action code for transition */
					this.display.setTT(document.timerMgr.getPresetInSec(this.tid));


					/* adjust state variables */
					this.stateVarsCopy.stateVarCookingRegion = this.states.TimeShown;
				} else {
					/* Intentionally left blank*/
				} /*end of event selection*/
			break; /* end of case TimeNotShown */

			default:
				/* Intentionally left blank*/
			break;
		} /* end switch stateVar_root*/

		return evConsumed;
	}



	// Region code for region MainRegion
	this.processEventMainRegion = function(msg, userData) {
		var evConsumed = 0;
	var eventConsumedCookingRegion = 0


		switch (this.stateVars.stateVarMainRegion) {

			case this.states.Completed:
				if (msg === this.events.evDoorOpen) {
					/* Transition from Completed to Idle*/
					evConsumed=1;

					/* Action code for transition */
					document.getElementById("myImage").src = "images/oven_open_off.png";


					/* adjust state variables */
					this.stateVarsCopy.stateVarMainRegion = this.states.Idle;
				} else {
					/* Intentionally left blank*/
				} /*end of event selection*/
			break; /* end of case Completed */

			case this.states.Cooking:
				/* calling region code */
				evConsumed |= this.processEventCookingRegion(msg, userData);

				/* Check if event was already processed */
				if(evConsumed===0){

					if (msg === this.events.evDoorOpen) {
						/* Transition from Cooking to CookingPause*/
						evConsumed=1;

						/* Exit code for regions in state Cooking*/
						
						/* Action code for transition */
						console.log("Oven Off");
						document.timerMgr.pauseTimer(this.tid);
						this.display.updateImage("images/oven_open_off.png");


						/* adjust state variables */
						this.stateVarsCopy.stateVarMainRegion = this.states.CookingPause;
					} else if (msg === this.events.evTimeout) {
						/* Transition from Cooking to Completed*/
						evConsumed=1;

						/* Exit code for regions in state Cooking*/
						
						/* Action code for transition */
						console.log("Oven Off");
						document.timerMgr.stopTimer(this.tid);
						document.timerMgr.stopTimer(this.tidBlink);
						this.display.updateImage("images/oven_closed_off.png");


						/* adjust state variables */
						this.stateVarsCopy.stateVarMainRegion = this.states.Completed;
					} else {
						/* Intentionally left blank*/
					} /*end of event selection*/
				}
			break; /* end of case Cooking */

			case this.states.CookingPause:
				if (msg === this.events.evDoorClosed) {
					/* Transition from CookingPause to Cooking*/
					evConsumed=1;

					/* Action code for transition */
					document.timerMgr.continueTimer(this.tid);
					this.display.updateImage("images/oven_closed_on.png");


					/* Entry code for regions in state Cooking*/
					/* entry chain */


					/* adjust state variables */
					this.stateVarsCopy.stateVarMainRegion = this.states.Cooking;
				} else {
					/* Intentionally left blank*/
				} /*end of event selection*/
			break; /* end of case CookingPause */

			case this.states.Idle:
				if (msg === this.events.evDoorClosed) {
					if (document.timerMgr.getPreset(this.tid)) {
						/* Transition from Idle to Cooking*/
						evConsumed=1;

						/* Action code for transition */
						document.timerMgr.startTimer(this.tid,false);
						document.timerMgr.startTimer(this.tidBlink,true);
						this.display.updateImage("images/oven_closed_on.png");


						/* Entry code for regions in state Cooking*/
						/* entry chain */


						/* adjust state variables */
						this.stateVarsCopy.stateVarMainRegion = this.states.Cooking;
					} else {
						/* Transition from Idle to Idle*/
						evConsumed=1;

						/* Action code for transition */
						this.display.updateImage("images/oven_closed_off.png");


						/* adjust state variables */
						this.stateVarsCopy.stateVarMainRegion = this.states.Idle;
					} /*end of event selection*/
				} else if (msg === this.events.evDoorOpen) {
					/* Transition from Idle to Idle*/
					evConsumed=1;

					/* Action code for transition */
					this.display.updateImage("images/oven_open_off.png");


					/* adjust state variables */
					this.stateVarsCopy.stateVarMainRegion = this.states.Idle;
				} else {
					/* Intentionally left blank*/
				} /*end of event selection*/
			break; /* end of case Idle */

			default:
				/* Intentionally left blank*/
			break;
		} /* end switch stateVar_root*/

		return evConsumed;
	}


}


