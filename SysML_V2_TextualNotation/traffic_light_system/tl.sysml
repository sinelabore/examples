private import ScalarValues::*;

package TrafficLight {


	// definition of events to control TrafficLightController
	enum def TLCEvent {
		evOperational;
		evError;
	}

	enum def TMCEvent {
	}

	item def PortData{
		attribute msg : TLCEvent;
	}


	port def ControlPortData {
		in item data:PortData;
	}

	part def Test{}

	// traffic management center
	part def TrafficManagementCenter{
		out port sendPort : ControlPortData;

		state def tmcStateMachine {
			entry; then PreOperational;

			state PreOperational;
			accept after 2[SI::second] do send TLCEvent::evOperational via sendPort then Operational;

			state Operational {

			}
		}
	}

	// template of a tlc part
	part def TrafficLightController{
		attribute redtime:Integer default 2;
		attribute msg:TLCEvent;	
		in port recvPort : ~ControlPortData;


		action def setRedLED{}
		action def resetRedLED{}
		action def setYellowLED{}
		action def resetYellowLED{}
		action def setGreenLED{}
		action def resetGreenLED{}
		action def setRedAndYellowLED{}
		action def resetRedAndYellowLED{}
		action def countServiceTime{}
	}

	part def BasicTrafficLightController:>TrafficLightController{
		// specific implementation of a tlc part
		// redefine the default value of redtimes
		attribute :>>redtime=1;

		// action to get the message from the control port
		action def getMsg{
			action accept m:PortData via recvPort{
				assign msg:=m.msg;
			}
		}

		state def tlcStateMachine parallel {
			do action getMsg{}
			state Activity{
				entry; then OutOfService;

				state  OutOfService {
					entry; then OutOfServiceYellowOn;

					state OutOfServiceYellowOn{
						entry action setYellowLED{}
					}
					accept after 0.5[SI::second] then OutOfServiceYellowOff;
					
					state OutOfServiceYellowOff {
						entry action resetYellowLED{}
					}
					accept after 0.5[SI::second] then OutOfServiceYellowOn;

					transition t1 first OutOfService accept when msg==TLCEvent::evOperational then Operational;
				}
					
				state Operational {
					entry; then OperationalRed;

					state OperationalRed {
						entry action setRedLED{}
						exit action resetRedLED{}
					}

					state OperationalRedYellow {
						entry action setRedAndYellowLED{}
						exit action resetRedAndYellowLED{}
					}

					state OperationalGreen {
						entry action setGreenLED{}
						exit action resetGreenLED{}
					}

					state OperationalYellow {
						entry action setYellowLED{}
						exit action resetYellowLED{}
					}

					transition t1 first OperationalRed  accept after redtime[SI::second] then OperationalRedYellow;
					transition t2 first OperationalRedYellow  accept after 1[SI::second] then OperationalGreen;
					transition t3 first OperationalGreen  accept after 1[SI::second] then OperationalYellow;
					transition t4 first OperationalYellow  accept after 1[SI::second] then OperationalRed;
					transition t5 first Operational if msg==TLCEvent::evError then OutOfService;
				}
			}
			state CountingServiceTime{ // State to count the service time
				entry; then CountServiceTime;

				state CountServiceTime{
					do action countServiceTime{}
				}

			}
		}
	}




	// Main system composition
	part def TrafficLightSystem {
		doc /*
		* Main system that contains the parts
		*/

		// one control center ...
		part tmc : TrafficManagementCenter;
		// ... manages two traffic lights
		part tlc1 : BasicTrafficLightController;
		part tlc2 : BasicTrafficLightController;
		
		// Connect the parts
		connect tmc.sendPort to tlc1.recvPort;
		connect tmc.sendPort to tlc2.recvPort;
	}
}
