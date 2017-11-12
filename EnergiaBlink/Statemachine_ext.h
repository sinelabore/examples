/* (C) Your company*//* Command line options: -Trace -l cppx -v -p ASTAH -o Statemachine -t Demo:SM Model.asta   */
/* This file is generated from Model.asta - do not edit manually  */
/* Generated on: Sun Nov 12 14:41:06 CET 2017 / version 3.7.2.1 */


#ifndef __STATEMACHINE_EXT_H__
#define __STATEMACHINE_EXT_H__

// Events which can be sent to the state-machine
enum STATEMACHINE_EVENT_T {
	evTimeout=0U,
	evButton2,
	evButton1,
	STATEMACHINE_NO_MSG
};




#define InnermostStates "FastLedOn,FastLedOff,Off,On,SlowWaitForLastTimeout,FastWaitForLastTimeout,SlowLedOn,SlowLedOff;"

#endif
