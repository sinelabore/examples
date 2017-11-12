/* (C) Your company*//* Command line options: -Trace -l cppx -v -p ASTAH -o Statemachine -t Demo:SM Model.asta   */
/* This file is generated from Statemachine_trace - do not edit manually  */
/* Generated on: version 3.7.2.1 */ 



/* Simulation events which can be sent to the state-machine statediagram.simulator */
class Statemachine{
	public String[] simulationEvents={
		"evTimeout",
		"evButton2",
		"evButton1"
	};


	String getEventFromRef(int ref){
		return simulationEvents[ref];
	}
}
