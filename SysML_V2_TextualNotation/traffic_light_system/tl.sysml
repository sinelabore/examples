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

	part def TrafficLightController{

		attribute redtime:Integer default 1;
		attribute msg:TLCEvent;

		in port recvPort : ~ControlPortData;

		action def getMsg{
			action accept m:PortData via recvPort{
				assign msg:=m.msg;
			}
		}

		action def setRedLED{}
		action def resetRedLED{}
		action def setYellowLED{}
		action def resetYellowLED{}
		action def setGreenLED{}
		action def resetGreenLED{}
		action def setRedAndYellowLED{}
		action def resetRedAndYellowLED{}

		state def tlcStateMachine {
			entry; then OutOfService;
			do action getMsg{}

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
	}




	// Main system composition
	part def TrafficLightSystem {
		doc /*
		* Main system that contains the parts
		*/
		part tmc : TrafficManagementCenter;
		part tlc : TrafficLightController;
		
		// Connect the parts
		connect tmc.sendPort to tlc.recvPort;
	}
}
