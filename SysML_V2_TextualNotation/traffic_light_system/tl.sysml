/*
 * Traffic light system - example model for the SysML v2 to C++ code generator.
 *
 * A traffic management center (TMC) supervises two traffic lights. The model
 * demonstrates parts with ports and connections, inheritance between parts,
 * parallel state machines and actions that drive parts of another definition.
 *
 * Structure
 *   TrafficLightSystem       one TMC (tmc) plus two traffic lights (tlc1, tlc2)
 *   TrafficManagementCenter  sends control events, receives service requests
 *   TrafficLightController   template: lamps, timing, the lamp switching actions
 *   BasicTrafficLightController :> TrafficLightController
 *                            adds the concrete state machine and shortens redtime
 *   Lamp                     from the Signalling package (separate file)
 *
 * Behaviour
 *   The TMC starts in PreOperational and after two seconds broadcasts
 *   TLCEvent::evOperational to both traffic lights over sendPort.
 *
 *   Each traffic light runs a parallel state machine with two regions:
 *
 *     Activity   Until the operational event arrives the light is OutOfService
 *                and blinks yellow at 0.5 s. On evOperational it enters
 *                Operational and cycles red -> red+yellow -> green -> yellow;
 *                the red phase lasts redtime seconds, the others one second.
 *                TLCEvent::evError sends it back to OutOfService.
 *
 *     CountingServiceTime
 *                Independently of the light phase, checkServiceCounter watches
 *                how often the lamps have been switched. Above the threshold
 *                needService is set, the light reports it to the TMC over
 *                servicePort, and the TMC schedules a service call.
 *
 * The lamps are modelled as a collection (lamps [*]) with redLamp, yellowLamp
 * and greenLamp as named members, so both individual and collective access is
 * possible. Their switch counters are what triggers the maintenance request.
 */

private import ScalarValues::*;
private import Signalling::*;

package TrafficLight {


	// definition of events to control TrafficLightController
	enum def TLCEvent {
		evOperational;
		evError;
	}

	enum def TMCEvent {
		evServiceNeeded;
	}

	item def ControlPortData{
		attribute msg : TLCEvent;
	}


	port def ControlPort {
		in item data:ControlPortData;
	}

	item def ServicePortData{
		attribute msg:TMCEvent;
	}

	port def ServicePort{
		in item data:ServicePortData;
	}

	// traffic management center in public utility
	part def TrafficManagementCenter{
		out port controlPort : ControlPort;
		in port servicePort : ~ServicePort;
		attribute msg:TMCEvent;

		// action to get the message from the control port
		action getMsg{
			action accept m:ServicePortData via servicePort{
				assign msg:=m.msg;
			}
		}

		state tmcStateMachine {
			entry; then PreOperational;

			do action getMsg;

			state PreOperational;
			accept after 2[SI::second] do send TLCEvent::evOperational via controlPort then Operational;

			state Operational;
			//if msg==TMCEvent::evServiceNeeded then OperationalWihtServiceNeed;
			accept when msg==TMCEvent::evServiceNeeded then OperationalWihtServiceNeed;

			state OperationalWihtServiceNeed{
				// send service Engineer
				entry action scheduleServiceCall;
			}
		}
	}

	// template of a tlc part
	part def TrafficLightController{

		attribute redtime:Integer default 2;
		attribute msg:TLCEvent;	
		attribute needService:Boolean default false;
		in port recvPort : ~ControlPort;
		out port servicePort : ServicePort;

        // collection of all lamps
        abstract ref part lamps : Lamp [*];
        // named members of that collection
        ref part redLamp subsets lamps : Lamp;
        ref part yellowLamp subsets lamps : Lamp;
        ref part greenLamp subsets lamps : Lamp;

		action setRed{
			first start;
			then action references redLamp.setOn;
			then done;
		}

		action resetRed{}

		action setYellow{
			first start;
			then action references yellowLamp.setOn;
			then done;
		}

		action resetYellow{
			first start;
			then perform yellowLamp.setOff; //PerformActionUsage default name setOff
			then done;
		}			

		action setGreen{
			first start;
			then action references greenLamp.setOn; // Anonymous ActionUsage with ReferenceSubsetting
			then done;
		}
		action resetGreen{
			first start;
			then perform greenLamp.setOff;
			then done;
		}			

		action setRedAndYellow{
			first start;
			then action ryOn references yellowLamp.setOn;
			then action ryOff references redLamp.setOn;
			then done;
		}

		action resetRedAndYellow{
			first start;
			then action references redLamp.setOff;
			then action references yellowLamp.setOn;
			then done;
		}			

		action checkServiceCounter{

			first start;
			if yellowLamp.switchCounter>5 or 
			   redLamp.switchCounter > 5 or
			   greenLamp.switchCounter > 5
			{
				assign needService:=true;
			}

		}
	}

	part def BasicTrafficLightController:>TrafficLightController{
		// specific implementation of a tlc part
		// redefine the default value of redtimes
		attribute :>>redtime=1;

		// action to get the message from the control port
		action getMsg{
			action accept m:ControlPortData via recvPort{
				assign msg:=m.msg;
			}
		}

		action sendServiceRequest{
			first start;
			then send TMCEvent::evServiceNeeded via servicePort;
		}

		state tlcStateMachine parallel {
			do action getMsg;
			state Activity{
				entry; then OutOfService;

				state  OutOfService {
					entry; then OutOfServiceYellowOn;

					state OutOfServiceYellowOn{
						entry action setYellow;
					}
					accept after 0.5[SI::second] then OutOfServiceYellowOff;
					
					state OutOfServiceYellowOff {
						entry action resetYellow;
					}
					accept after 0.5[SI::second] then OutOfServiceYellowOn;

					transition t1 first OutOfService accept when msg==TLCEvent::evOperational then Operational;
				}
					
				state Operational {
					entry; then OperationalRed;

					state OperationalRed {
						entry action setRed;
						exit action resetRed;
					}

					state OperationalRedYellow {
						entry action setRedAndYellow;
						exit action resetRedAndYellow;
					}

					state OperationalGreen {
						entry action setGreen;
						exit action resetGreen;
					}

					state OperationalYellow {
						entry action setYellow;
						exit action resetYellow;
					}

					transition t1 first OperationalRed  accept after redtime[SI::second] then OperationalRedYellow;
					transition t2 first OperationalRedYellow  accept after 1[SI::second] then OperationalGreen;
					transition t3 first OperationalGreen  accept after 1[SI::second] then OperationalYellow;
					transition t4 first OperationalYellow  accept after 1[SI::second] then OperationalRed;
					transition t5 first Operational if msg==TLCEvent::evError then OutOfService;
				}
			}
			state CountingServiceTime{ // State to check the service time
				entry; then noService;

				state serviceNeeded;
					if  needService==false then noService;

				state noService{
					do action references checkServiceCounter;
				}
				transition t1 first noService if  needService==true do action references sendServiceRequest then serviceNeeded;
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
		
		// Connect the parts via its ports
		connect tmc.controlPort to tlc1.recvPort;
		connect tmc.controlPort to tlc2.recvPort;
		connect tlc2.servicePort to tmc.servicePort;
		connect tlc1.servicePort to tmc.servicePort;
	}
}
