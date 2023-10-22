/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2022
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -Trace -l cppx -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven_trace - do not edit manually  */
/* Generated on: Version 6.0.2.3601 */ 



/* Simulation events which can be sent to the state-machine statediagram.simulator */
class oven{
	public String[] simulationEvents={
	"evDec",
	"evInc",
	"evDoorClosed[timer.preset()>0]",
	"evDoorOpen[timer.preset()==0]",
	"evPwrLow",
	"evPwrHigh",
	"evTimeout",
	"evDoorOpen",
	"evDoorClosed",
	"__INIT__"
	};


	String getEventFromRef(int ref){
		return simulationEvents[ref];
	}
}
