private import SI::*;
private import ScalarValues::*;

// model of a microwave ofen and its components
package MicrowaveOfen{
    
    part def ElectricPart{
        attribute power : Real default 0 [SI::watt];
    }

    enum def WheelCmd{
        INCREMENT;
        DECREMENT;
    }

    enum def Commands{
        ON;
        OFF;
        PAUSE;
    }

    item def CommandData{
        attribute cmd : Commands;
    }

    item def WheelData{
        attribute tick : WheelCmd;
    }

    port def CommandPort{
        out item cmd : CommandData;
    }

    port def WheelPort{
        out item cmd : WheelData;
    }

    part def Motor :> ElectricPart{
        :>> power default 5[SI::watt];
        port cmd : ~CommandPort;
    }

    part def Controller :> ElectricPart{
        :>> power default 0.1[SI::Watt];

        out port cmdPortToLamp : CommandPort;
        out port cmdPortToMotor : CommandPort;
        out port cmdPortToMagnetron : CommandPort;
        in port timePort : ~CommandPort;

		action run{
			action accept m:CommandData via timePort{
                if m.cmd==Commands::ON{
                    then send new CommandData(Commands::ON) via cmdPortToLamp;
                    then send new CommandData(Commands::ON) via cmdPortToMotor;
                    then send new CommandData(Commands::ON) via cmdPortToMagnetron;
                } else if m.cmd==Commands::OFF{
                    then send new CommandData(Commands::OFF) via cmdPortToLamp;
                    then send new CommandData(Commands::OFF) via cmdPortToMotor;
                    then send new CommandData(Commands::OFF) via cmdPortToMagnetron;
                } else if m.cmd==Commands::PAUSE{
                    then send new CommandData(Commands::OFF) via cmdPortToMotor;
                    then send new CommandData(Commands::OFF) via cmdPortToMagnetron;                    
                }else{
                    // should never happen
                }

			}
		}

        // 'run' waits for a command on timePort; a state's 'do' action is what runs on
        // every scan, and the part's generated process() steps this machine.
        state ControllerState{
            entry; then RUNNING;
            state RUNNING{
                do run;
            }
        }
    }

    part def Lamp :> ElectricPart{
        :>> power default 5[SI::watt];
        port cmd : ~CommandPort;
    }

    part def Magnetron :> ElectricPart{
        :>> power default 1000[SI::watt];
        port cmd : ~CommandPort;
    }


    //
    part def TimerDisplay :> ElectricPart{
        in port doorSignal : ~CommandPort;
        out port timePort : CommandPort;
        in port wheelPort : ~WheelPort;
        :>> power default 0.04[SI::watt];

        attribute adjustedTicks : Real default 0[SI::second];
        item doorState:CommandData;

		action readDoorSignal{
			action accept m:CommandData via doorSignal{
				assign doorState:=m.cmd;
			}
		}

		action readTickSignal{
			action accept m:WheelData via wheelPort{
				if m.tick==WheelCmd::INCREMENT{
                    assign adjustedTicks:=adjustedTicks+1;
                }else if m.tick==WheelCmd::DECREMENT{
                    if(adjustedTicks>0){
                        assign adjustedTicks:=adjustedTicks-1;
                    }
                }else{
                    // should never happen
                }
			}
		}

        action doCountDown{
            if(adjustedTicks>0){
                assign adjustedTicks:=adjustedTicks-1;
            }            
        }


		state TimerDisplayState{

            first start then ZERO;
            do readTickSignal{}

            state ZERO{
                entry action { send new CommandData(Commands::OFF) via timePort; }
            }
            accept m:CommandData via doorSignal if m.cmd==Commands::ON and adjustedTicks>0 then COUNTDOWN;

            state PAUSE{
                entry action { send new CommandData(Commands::PAUSE) via timePort; }
            }
            if adjustedTicks==0 then ZERO;
            accept m:CommandData via doorSignal if m.cmd==Commands::ON and adjustedTicks>0 then COUNTDOWN;


            state COUNTDOWN{
                do doCountDown{}
                entry action { send new CommandData(Commands::ON) via timePort; }
            }
            if adjustedTicks==0 then ZERO;
            accept m:CommandData via doorSignal if m.cmd==Commands::OFF then PAUSE;
        }
    }

    // wheel to increase / decrese time. The wheel does know nothing on the ajusted time.
    part def Wheel{
        port wheel : WheelPort;
        
        action doInc{
            then send new WheelData(WheelCmd::INCREMENT) via wheel;
        }

        action doDec{
            then send new WheelData(WheelCmd::DECREMENT) via wheel;
        }
    }

    part def DoorContact{
        port doorSignal : CommandPort;

        attribute contact : Boolean default false;

        action readContact{
            if contact==true{
                then send new CommandData(Commands::ON) via doorSignal;
            }else {
                then send new CommandData(Commands::OFF) via doorSignal;
            }
        }

        // The contact is read on every scan, which is what a state's 'do' action means.
        // The part's generated process() steps this machine, so nothing outside has to
        // know that readContact exists.
        state DoorContactState{
            entry; then CLOSED;
            state CLOSED{
                do readContact;
            }
        }
    }

    part Oven{
        abstract ref part electric : ElectricPart[*];
        
        part motor : Motor subsets electric;

        part controller : Controller subsets electric;
        part lamp : Lamp subsets electric;
        part magnetron : Magnetron subsets electric;
        part timerDisplay : TimerDisplay subsets electric;
        
        part doorContact : DoorContact;
        part wheel:Wheel;

        attribute nominalPower : Real = sum(electric.power);

        connect controller.cmdPortToMagnetron to magnetron.cmd;
        connect controller.cmdPortToMotor to motor.cmd;
        connect controller.cmdPortToLamp to lamp.cmd;
        connect doorContact.doorSignal to timerDisplay.doorSignal;
        connect timerDisplay.timePort to controller.timePort;
        connect wheel.wheel to timerDisplay.wheelPort;
    }
}
