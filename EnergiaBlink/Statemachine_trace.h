/* (C) Your company*//* Command line options: -Trace -l cppx -v -p ASTAH -o Statemachine -t Demo:SM Model.asta   */
/* This file is generated from Statemachine_trace - do not edit manually  */
/* Generated on: version 3.7.2.1 */ 


#ifndef __STATEMACHINE_TRACE_H__
#define __STATEMACHINE_TRACE_H__

/*Simulation which can be sent to the state-machine statediagram.simulator  */
enum StatemachineTraceEvent { STATEMACHINE_TRACE_EVENT_LEN=3};

void StatemachineTraceEvent(int evt);

const char* const StatemachineTraceEvents[] = {
	"evTimeout",
	"evButton2",
	"evButton1"
};


#endif
