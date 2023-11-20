package main

import (
	"fmt"
	"time"
)

// Demonstrates how to embedded the statemachine type into another type
// and use data/functions from this type e.g. as guards ... in the statemachine.
type Reactor struct {
	id           uint8
	idBlink      uint8
	Ticker       *time.Ticker
	Timers       *CountdownTimers
	Statemachine *Oven
}

// Creation function
func NewReactor() *Reactor {
	return &Reactor{
		id:           0,                    // You may set an appropriate default value for id
		Ticker:       nil,                  // You may initialize Ticker as needed
		Timers:       NewCountdownTimers(), // Assuming NewCountdownTimers is another function you've defined
		Statemachine: &Oven{},              // Assuming NewOven is a function to initialize the Oven state machine
	}
}

// React should run in a go coroutine and loops endless until it is
// stopped. In the loop three cases are considered:
// 1) timeouts of any active countdown_timer based on the system tick of 100ms
// 2) event cannel feeded from the main function with keyboard input
// 3) the quit channel if the program shout end
func (smBase *Reactor) React(c chan OvenEvent, quit chan bool) {
	smBase.Ticker = time.NewTicker(time.Millisecond * 100)
	var err error = nil
	for {
		select {
		case event := <-c: // receive events
			_, err = smBase.ProcessEvent(event)
		case <-quit: // receive quit event
			fmt.Println("Quit Co-routine")
			return
		case <-smBase.Ticker.C: // timer tick to check if there are any timeouts
			//debugPrint("Tick")
			vals := smBase.Timers.Tick()
			for i := 0; i < len(vals); i++ {
				//fmt.Printf("Timeout for timer %d\n", vals[i].id)
				if evt, ok := vals[i].Event.(OvenEvent); ok {
					_, err = smBase.ProcessEvent(evt)
				}
			}
		}
		if err != nil {
			debugPrint(fmt.Sprintf("internal error in sm %s\n", err))

			return
		}
	}
}

// called by the state machine if tracing is enabled
func (smBase *Reactor) TraceEvent(traceIdx int) {
	var traceString = smBase.Statemachine.TraceLookup(traceIdx)
	debugPrint(fmt.Sprintf("Trace msg: %s\n", traceString))
}

// called by the state machine if validation hander is enabled
func (smBase *Reactor) ValidationHandler(from OvenState, to OvenState) {
	toString, _ := smBase.Statemachine.StateToString(to)
	fromString, _ := smBase.Statemachine.StateToString(from)
	debugPrint(fmt.Sprintf("Change state from %s to %s\n", fromString, toString))
}

// simulate oven
func (smBase *Reactor) OvenOff() {
	debugPrint("Oven off")
}

// simulate oven
func (smBase *Reactor) OvenOn() {
	debugPrint("Oven on")
}
