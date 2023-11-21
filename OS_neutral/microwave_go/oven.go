/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -Trace -p ssc -l go -o oven oven.xml  */
/* This file is generated from oven.xml - do not edit manually */
/* Generated on: Tue Nov 21 18:30:08 CET 2023 / Version 6.2.3735*/



package main

import (
		"fmt"
        "errors"
)

type OvenEvent int
type OvenState int

// States of the state machine 
const (
	Error OvenState = iota
	Error1
	Super
	Completed
	Cooking
	CookingPause
	Idle
	LedOff
	LedOn
)

// Events the state machine can process
const (
	EvDec OvenEvent = iota
	EvTimeout
	EvDoorClosed
	EvTimeoutBlink
	EvPwr
	EvDoorOpen
	EvError
	EvInc
	OVEN_NO_MSG
)

// State lookup table
var stateLookup = map[OvenState]string {
	Error: "Error",
	Error1: "Error1",
	Super: "Super",
	Completed: "Completed",
	Cooking: "Cooking",
	CookingPause: "CookingPause",
	Idle: "Idle",
	LedOff: "LedOff",
	LedOn: "LedOn",
}

// Event lookup table
var eventLookup = map[OvenEvent]string {
	EvDec: "EvDec",
	EvTimeout: "EvTimeout",
	EvDoorClosed: "EvDoorClosed",
	EvTimeoutBlink: "EvTimeoutBlink",
	EvPwr: "EvPwr",
	EvDoorOpen: "EvDoorOpen",
	EvError: "EvError",
	EvInc: "EvInc",
	OVEN_NO_MSG: "OVEN_NO_MSG",
}

// Trace lookup table
var TraceLookupSlice = []string{
	"EvInc",
	"EvDec",
	"EvPwr",
	"EvError",
	"EvDoorOpen",
	"EvTimeout",
	"EvDoorClosed",
	"EvDoorClosed[smBase.Timers.GetPresetInSec(smBase.id)>0]",
	"EvDoorClosed[smBase.Timers.GetPresetInSec(smBase.id)==0]",
	"EvTimeoutBlink",
	"__INIT__",
}

// TraceLookup maps an index to a string representing the event and guard.
// The intended use is in the user written trace function e.g. for logging purposes.
func (sm *Oven) TraceLookup(idx int) string {
	return TraceLookupSlice[idx]
}
// Type of the state machine instance data
type Oven struct {
	isInit bool
	stateVar OvenState
	stateVarMainRegion OvenState
	stateVarNewRegion1 OvenState
	stateVarError OvenState
}

// StateToString maps a state to the state name string
func (sm *Oven) StateToString(state OvenState) (string, error) {
	value, hasKey := stateLookup[state]
	if hasKey {
		return value, nil
	} else {
		return "", errors.New("requesting state name for unknown state")
	}
}

// EventToString maps an event to the event name string
func (sm *Oven) EventToString(event OvenEvent) (string, error) {
	value, hasKey := eventLookup[event]
	if hasKey {
		return value, nil
	} else {
		return "", errors.New("requesting event name for unknown event")
	}
}

func (sm *Oven) IsInError() (bool) { return (sm.stateVar == Error)}
func (sm *Oven) IsInError1() (bool) { return (sm.stateVarError == Error1)&&(sm.stateVar == Error)}
func (sm *Oven) IsInSuper() (bool) { return (sm.stateVar == Super)}
func (sm *Oven) IsInCompleted() (bool) { return (sm.stateVarMainRegion == Completed) && sm.IsInSuper()}
func (sm *Oven) IsInCooking() (bool) { return (sm.stateVarMainRegion == Cooking) && sm.IsInSuper()}
func (sm *Oven) IsInCookingPause() (bool) { return (sm.stateVarMainRegion == CookingPause) && sm.IsInSuper()}
func (sm *Oven) IsInIdle() (bool) { return (sm.stateVarMainRegion == Idle) && sm.IsInSuper()}
func (sm *Oven) IsInLedOff() (bool) { return (sm.stateVarNewRegion1 == LedOff) && sm.IsInCooking()}
func (sm *Oven) IsInLedOn() (bool) { return (sm.stateVarNewRegion1 == LedOn) && sm.IsInCooking()}

// Initialize state machine
func (smBase *Reactor) Init() (error) {

	var sm = smBase.Statemachine

	var err error = nil
	// Set state vars to default states
	sm.stateVar = Super /* set init state of top state */
	sm.stateVarMainRegion = Idle; /* set init state of MainRegion */
	sm.stateVarNewRegion1 = LedOn; /* set init state of NewRegion1 */
	sm.stateVarError = Error1; /* set init state of Error */

	sm.isInit = true
	smBase.id = smBase.Timers.Create(EvTimeout, true)
	smBase.idBlink = smBase.Timers.Create(EvTimeoutBlink, false)
	smBase.OvenOff();
	

	return err
}
// Copy function of state machine data
func (o *Oven) Copy() *Oven {
	return &Oven {
		stateVar : o.stateVar,
		stateVarMainRegion : o.stateVarMainRegion,
		stateVarNewRegion1 : o.stateVarNewRegion1,
		stateVarError :  o.stateVarError,

		isInit : o.isInit,
		}
	}

// Copy back function of state machine data
func (sm *Oven) copyBack(copy *Oven) {
	sm.stateVar = copy.stateVar
	sm.stateVarMainRegion = copy.stateVarMainRegion
	sm.stateVarNewRegion1 = copy.stateVarNewRegion1
	sm.stateVarError =  copy.stateVarError

	sm.isInit = copy.isInit
}


// State machine event handler
func (smBase *Reactor) ProcessEvent(msg OvenEvent) (int, error) {
	var err error = nil
	var eventConsumed int = 0
	var eventConsumedMainRegion int = 0

	var sm = smBase.Statemachine

	if !sm.isInit {
		err = smBase.Init()
		if err != nil {
			return -1, err
		}
	}


	/* Create a copy of the instance data.
	   Changes during the machine execution are done on the copy 
	   while tests (e.g. isIn) must use the unmodified instance data */
	var smCopy *Oven= sm.Copy();

	/* action code */
	// prefix code would go here



	switch sm.stateVar {

		case Super:
			/* calling region code */
			eventConsumedMainRegion, err = sm.processEventMainRegion(msg, smCopy, smBase)
			if err != nil {
				return 0, err
			}
			
			eventConsumed |= eventConsumedMainRegion


			/* Check if event was already processed */
			if eventConsumed == 0 {

				if msg == EvDec {
					/* Transition from Super to Super*/
					smBase.ValidationHandler(Super, Super)
					eventConsumed=1

					/* Exit code for regions in state Super*/
					
					if(sm.stateVarMainRegion==Cooking){
						smBase.Timers.Stop(smBase.idBlink)
						fmt.Println("LED Off")

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition */
					smBase.Timers.Decrement(smBase.id,1000)


					/* Entry code for regions in state Super*/
					smBase.OvenOff();
					/* Default in entry chain */
					smCopy.stateVarMainRegion = Idle;/* Default in entry chain */


					/* adjust state variables */
					smCopy.stateVar = Super;
					smBase.TraceEvent(1)
				}else if msg == EvError {
					/* Transition from Super to Error*/
					smBase.ValidationHandler(Super, Error)
					eventConsumed=1

					/* Exit code for regions in state Super*/
					
					if(sm.stateVarMainRegion==Cooking){
						smBase.Timers.Stop(smBase.idBlink)
						fmt.Println("LED Off")

					}else {
						/* Intentionally left blank */
					};


					smCopy.stateVar = Error;/* Default in entry chain */
					smCopy.stateVarError = Error1;/* Default in entry chain */

					smBase.TraceEvent(3)
				}else if msg == EvInc {
					/* Transition from Super to Super*/
					smBase.ValidationHandler(Super, Super)
					eventConsumed=1

					/* Exit code for regions in state Super*/
					
					if(sm.stateVarMainRegion==Cooking){
						smBase.Timers.Stop(smBase.idBlink)
						fmt.Println("LED Off")

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition */
					smBase.Timers.Increment(smBase.id,1000)


					/* Entry code for regions in state Super*/
					smBase.OvenOff();
					/* Default in entry chain */
					smCopy.stateVarMainRegion = Idle;/* Default in entry chain */


					/* adjust state variables */
					smCopy.stateVar = Super;
					smBase.TraceEvent(0)
				}else if msg == EvPwr {
					/* Transition from Super to Super*/
					smBase.ValidationHandler(Super, Super)
					eventConsumed=1

					/* Exit code for regions in state Super*/
					
					if(sm.stateVarMainRegion==Cooking){
						smBase.Timers.Stop(smBase.idBlink)
						fmt.Println("LED Off")

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition */
					//ovenSetPwr(pwr);


					/* Entry code for regions in state Super*/
					smBase.OvenOff();
					/* Default in entry chain */
					smCopy.stateVarMainRegion = Idle;/* Default in entry chain */


					/* adjust state variables */
					smCopy.stateVar = Super;
					smBase.TraceEvent(2)
				}else{
					/* Intentionally left blank*/
				} /*end of event selection*/
			}
		break; /* end of case Super */

		case Error:

			switch sm.stateVarError {

				case Error1:
				break; /* end of case Error1 */

				default:
					/* Intentionally left blank*/
				break;
			} /* end switch Error*/
		break; /* end of case Error */

		default:
			/* Intentionally left blank*/
		break;
	} /* end switch stateVar_root*/

	// Copy state variables back
	sm.copyBack(smCopy);

	// Post Action Code
		// postfix code would go here

	return eventConsumed, err
}




// Region code for state MainRegion
func (sm *Oven) processEventMainRegion(msg OvenEvent, smCopy* Oven, smBase *Reactor) (int, error) {
	var eventConsumed int = 0
	var eventConsumedNewRegion1 int = 0

	var err error = nil

	switch sm.stateVarMainRegion {

		case Completed:
			if msg == EvDoorOpen {
				/* Transition from Completed to Idle*/
				smBase.ValidationHandler(Completed, Idle)
				eventConsumed=1

				/* OnEntry code of state Idle*/
				smBase.OvenOff();

				/* adjust state variables */
				smCopy.stateVarMainRegion = Idle;
				smBase.TraceEvent(4)
			}else{
				/* Intentionally left blank*/
			} /*end of event selection*/
		break; /* end of case Completed */

		case Cooking:
			/* calling region code */
			eventConsumedNewRegion1, err = sm.processEventNewRegion1(msg, smCopy, smBase)
			if err != nil {
				return 0, err
			}
			
			eventConsumed |= eventConsumedNewRegion1


			/* Check if event was already processed */
			if eventConsumed == 0 {

				if msg == EvDoorOpen {
					/* Transition from Cooking to CookingPause*/
					smBase.ValidationHandler(Cooking, CookingPause)
					eventConsumed=1

					/* Exit code for regions in state Cooking*/
					
					smBase.Timers.Stop(smBase.idBlink)
					fmt.Println("LED Off")

					/* Action code for transition */
					smBase.OvenOff();
					smBase.Timers.Pause(smBase.id)


					/* adjust state variables */
					smCopy.stateVarMainRegion = CookingPause;
					smBase.TraceEvent(4)
				}else if msg == EvTimeout {
					/* Transition from Cooking to Completed*/
					smBase.ValidationHandler(Cooking, Completed)
					eventConsumed=1

					/* Exit code for regions in state Cooking*/
					
					smBase.Timers.Stop(smBase.idBlink)
					fmt.Println("LED Off")

					/* Action code for transition */
					smBase.OvenOff();
					smBase.Timers.Stop(smBase.id);


					/* adjust state variables */
					smCopy.stateVarMainRegion = Completed;
					smBase.TraceEvent(5)
				}else{
					/* Intentionally left blank*/
				} /*end of event selection*/
			}
		break; /* end of case Cooking */

		case CookingPause:
			if msg == EvDoorClosed {
				/* Transition from CookingPause to Cooking*/
				smBase.ValidationHandler(CookingPause, Cooking)
				eventConsumed=1

				/* Action code for transition */
				smBase.Timers.Resume(smBase.id)

				/* OnEntry code of state Cooking*/
				smBase.OvenOn();

				/* Entry code for regions in state Cooking*/
				smBase.Timers.Start(smBase.idBlink,500)
				fmt.Println("LED on")
				/* Default in entry chain */
				smCopy.stateVarNewRegion1 = LedOn;/* Default in entry chain */


				/* adjust state variables */
				smCopy.stateVarMainRegion = Cooking;
				smBase.TraceEvent(6)
			}else{
				/* Intentionally left blank*/
			} /*end of event selection*/
		break; /* end of case CookingPause */

		case Idle:
			if msg == EvDoorClosed {
				if smBase.Timers.GetPresetInSec(smBase.id)==0 {
					/* Transition from Idle to Idle*/
					smBase.ValidationHandler(Idle, Idle)
					eventConsumed=1

					/* Action code for transition */
					fmt.Println("Time zero, increase cooking time")

					/* OnEntry code of state Idle*/
					smBase.OvenOff();

					/* adjust state variables */
					smCopy.stateVarMainRegion = Idle;
					smBase.TraceEvent(8)
				}else if smBase.Timers.GetPresetInSec(smBase.id)>0 {
					/* Transition from Idle to Cooking*/
					smBase.ValidationHandler(Idle, Cooking)
					eventConsumed=1

					/* Action code for transition */
					smBase.Timers.StartWithPreset(smBase.id)

					/* OnEntry code of state Cooking*/
					smBase.OvenOn();

					/* Entry code for regions in state Cooking*/
					smBase.Timers.Start(smBase.idBlink,500)
					fmt.Println("LED on")
					/* Default in entry chain */
					smCopy.stateVarNewRegion1 = LedOn;/* Default in entry chain */


					/* adjust state variables */
					smCopy.stateVarMainRegion = Cooking;
					smBase.TraceEvent(7)
				}else{
					/* Intentionally left blank*/
				} /*end of event selection*/
			}else{
				/* Intentionally left blank*/
			} /*end of event selection*/
		break; /* end of case Idle */

		default:
			/* Intentionally left blank*/
		break;
	} /* end switch stateVar_root*/

	return eventConsumed, err
}




// Region code for state NewRegion1
func (sm *Oven) processEventNewRegion1(msg OvenEvent, smCopy* Oven, smBase *Reactor) (int, error) {
	var eventConsumed int = 0

	var err error = nil

	switch sm.stateVarNewRegion1 {

		case LedOn:
			if msg == EvTimeoutBlink {
				/* Transition from LedOn to LedOff*/
				smBase.ValidationHandler(LedOn, LedOff)
				eventConsumed=1

				/* OnEntry code of state LedOff*/
				fmt.Println("LED off")

				/* adjust state variables */
				smCopy.stateVarNewRegion1 = LedOff;
				smBase.TraceEvent(9)
			}else{
				/* Intentionally left blank*/
			} /*end of event selection*/
		break; /* end of case LedOn */

		case LedOff:
			if msg == EvTimeoutBlink {
				/* Transition from LedOff to LedOn*/
				smBase.ValidationHandler(LedOff, LedOn)
				eventConsumed=1

				/* OnEntry code of state LedOn*/
				fmt.Println("LED on")

				/* adjust state variables */
				smCopy.stateVarNewRegion1 = LedOn;
				smBase.TraceEvent(9)
			}else{
				/* Intentionally left blank*/
			} /*end of event selection*/
		break; /* end of case LedOff */

		default:
			/* Intentionally left blank*/
		break;
	} /* end switch stateVar_root*/

	return eventConsumed, err
}


