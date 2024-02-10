/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -Trace -p ssc -l mjs oven.xml  */

import { TimerManager } from './TimerManager.mjs';
import { DisplayInterface } from './DisplayInterface.mjs';


/**
 * oven statemachine implementation 
 * This file is generated from oven.xml - do not edit manually 
 * Generated: Sat Feb 10 15:43:06 CET 2024 / Version 6.3.1.3798
 * @constructor
*/
export function oven() {

	this.init =  false;
	this.instId = 0;
	/* State machine state data */
	this.stateVars = {
		stateVar: null,
		stateVarMainRegion : null,
		stateVarCookingRegion : null,
	};

	/** States of the state machine */
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

	/** Events the state machine can process */
	this.events = Object.freeze({
			none: 0,
			evTimeout: 1,
			evDec: 2,
			evDoorOpen: 3,
			evDoorClosed: 4,
			evInc: 5,
			evBlink: 6,
		})


	/** Strings to be used for lookup in trace code for example 
	 * @example
	 * myMachine.traceFunction(instId, ev){
	 *   console.log("trace event =" + myMachine.traceEventStrings[ev]);
	 * }
	 */
	this.traceEventStrings = Object.freeze([
		"evInc",
		"evDec",
		"evDoorOpen",
		"evTimeout",
		"evDoorClosed",
		"evDoorClosed[document.timerMgr.getPreset(this.tid)]",
		"evDoorClosed[else]",
		"evBlink",
		"__INIT__"
	]);

	/** Initialize state machine one after creation */
	this.initialize = function(){
		this.ovenTraceEvent(this.instId,8);
		this.init = true;
		// Set state vars to default states
		this.stateVars.stateVar = this.states.Super /* set init state of top state */
		this.stateVars.stateVarMainRegion = this.states.Idle; /* set init state of MainRegion */
		this.stateVars.stateVarCookingRegion = this.states.TimeShown; /* set init state of CookingRegion */

		this.tid = document.timerMgr.createTimer(0, this.events.evTimeout);
		this.tidBlink = document.timerMgr.createTimer(500, this.events.evBlink);
		this.display = new DisplayInterface();

	}

	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInSuper = function() { return (this.stateVars.stateVar === this.states.Super);}
	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInTimeNotShown = function() { return (this.stateVars.stateVarCookingRegion === this.states.TimeNotShown) && this.isInCooking();}
	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInTimeShown = function() { return (this.stateVars.stateVarCookingRegion === this.states.TimeShown) && this.isInCooking();}
	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInCompleted = function() { return (this.stateVars.stateVarMainRegion === this.states.Completed) && this.isInSuper();}
	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInCooking = function() { return (this.stateVars.stateVarMainRegion === this.states.Cooking) && this.isInSuper();}
	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInCookingPause = function() { return (this.stateVars.stateVarMainRegion === this.states.CookingPause) && this.isInSuper();}
	/** 
	 * Find out if the machine is in a certain state
	 * @returns {boolean} true if the machine is in this state. False otherwise
	 */ 
	this.isInIdle = function() { return (this.stateVars.stateVarMainRegion === this.states.Idle) && this.isInSuper();}

	/** Return the state name string based on the state value */
	this.stateToString = function(stateValue) {
		const value = Object.keys(this.states).find(key => this.states[key] === stateValue);
		return value || `UnknownState(${stateValue})`;
	}

	/** Return the event name string based on the event value */
	this.eventToString = function(eventValue) {
		const value = Object.keys(this.events).find(key => this.events[key] === eventValue);
		return value || `UnknownEvent(${eventValue})`;
	}


	/** 
	 * Return of a map with the states in which the state machine is currently in 
	 * @returns {Map} Map with more or more states
	 */ 
	this.innermostActiveStates = function() {
		var statesMap = new Map();
		if ( this.isInCompleted() ) {statesMap.set(this.states.Completed,"Completed");}
		if ( this.isInTimeShown() ) {statesMap.set(this.states.TimeShown,"TimeShown");}
		if ( this.isInTimeNotShown() ) {statesMap.set(this.states.TimeNotShown,"TimeNotShown");}
		if ( this.isInCookingPause() ) {statesMap.set(this.states.CookingPause,"CookingPause");}
		if ( this.isInIdle() ) {statesMap.set(this.states.Idle,"Idle");}
		return statesMap;
	}


	/** 
	 * State machine event handler
	 * @param {events} msg is the event from the available events to be processed by the state machine
	 * @param {object} userData can be any user provided data for use in the state machine (e.g. as guard)
	 * @returns {number} Value indicates whether the event could be processed.
	 * 0=not processed
	 * 1=processed
	 * 16=conditional event processed
	 */ 
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
						this.ovenTraceEvent(this.instId, 1);
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
						this.ovenTraceEvent(this.instId, 0);
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



	/**
	 * Region code for region used internally by the event handlerCookingRegion
	 * @private
	 */
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
					this.ovenCookingRegionTraceEvent(this.instId, 7);
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
					this.ovenCookingRegionTraceEvent(this.instId, 7);
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



	/**
	 * Region code for region used internally by the event handlerMainRegion
	 * @private
	 */
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
					this.ovenMainRegionTraceEvent(this.instId, 2);
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
						this.ovenMainRegionTraceEvent(this.instId, 2);
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
						this.ovenMainRegionTraceEvent(this.instId, 3);
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
					this.ovenMainRegionTraceEvent(this.instId, 4);
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
						this.ovenMainRegionTraceEvent(this.instId, 5);
					} else {
						/* Transition from Idle to Idle*/
						evConsumed=1;

						/* Action code for transition */
						this.display.updateImage("images/oven_closed_off.png");


						/* adjust state variables */
						this.stateVarsCopy.stateVarMainRegion = this.states.Idle;
						this.ovenMainRegionTraceEvent(this.instId, 6);
					} /*end of event selection*/
				} else if (msg === this.events.evDoorOpen) {
					/* Transition from Idle to Idle*/
					evConsumed=1;

					/* Action code for transition */
					this.display.updateImage("images/oven_open_off.png");


					/* adjust state variables */
					this.stateVarsCopy.stateVarMainRegion = this.states.Idle;
					this.ovenMainRegionTraceEvent(this.instId, 2);
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


