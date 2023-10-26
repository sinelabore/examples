package oven.Model;

/* (C) Sinelabore Software Tools GmbH */
/* Command line options: -p ssc -E -l java -U ./codegen.cfg -o Reactor Reactor.xml   */
/* This file is generated from Reactor.xml - do not edit manually  */
/* Generated on: Thu Oct 26 20:49:47 CEST 2023 / Version 6.1.3706 */


import oven.*;

public class Reactor extends ReactorBase
{

	public enum States {
		Completed,
		Cooking,
		CookingPause,
		Idle,
		LedOff,
		LedOn,
		Super,
		__UNKNOWN_STATE__
	}

	public enum Events  {
		evPwrLow,
		evTimeout,
		evDec,
		evDoorOpen,
		evDoorClosed,
		evPwrHigh,
		evTimeoutLed,
		evInc,
		REACTOR_NO_MSG
	}


	// flag if initialized
	protected boolean m_initialized=false;

	States stateVar;
	States stateVarSuper;
	States stateVarCooking;

	// State handler class default ctor
	public Reactor()
	{
	}

	/* Helper(s) to reset history */
	public void resetHistorySuper(){stateVarSuper=States.Idle;}
	public void resetHistoryCooking(){stateVarCooking=States.LedOff;}

	/* Helper to get innermost active state id */
	public States getInnermostActiveState() {
		if(isInLedOn()){
			return States.LedOn;
		}else if(isInLedOff()){
			return States.LedOff;
		}else if(isInIdle()){
			return States.Idle;
		}else if(isInCookingPause()){
			return States.CookingPause;
		}else if(isInCompleted()){
			return States.Completed;
		}else{
			return States.__UNKNOWN_STATE__;
		}
	}

	// Helper(s) to find out if the machine is in a certain state
	public boolean isInCompleted(){return (((stateVarSuper== States.Completed)&&(stateVar== States.Super)) ? (true) : (false));}
	public boolean isInCooking(){return (((stateVarSuper== States.Cooking)&&(stateVar== States.Super)) ? (true) : (false));}
	public boolean isInCookingPause(){return (((stateVarSuper== States.CookingPause)&&(stateVar== States.Super)) ? (true) : (false));}
	public boolean isInIdle(){return (((stateVarSuper== States.Idle)&&(stateVar== States.Super)) ? (true) : (false));}
	public boolean isInLedOff(){return (((stateVarCooking== States.LedOff)&&(stateVarSuper== States.Cooking)&&(stateVar== States.Super)) ? (true) : (false));}
	public boolean isInLedOn(){return (((stateVarCooking== States.LedOn)&&(stateVarSuper== States.Cooking)&&(stateVar== States.Super)) ? (true) : (false));}
	public boolean isInSuper(){return (((stateVar== States.Super)) ? (true) : (false));}




	// Reinitialize the state machine
	public void reInitialize(){
			m_initialized=false;
			initialize();
	}

	public void initialize(){

		if(m_initialized==false){

			m_initialized=true;

			// Set state vars to default states
			stateVar = States.Super; /* set init state of top state */
			stateVarSuper = States.Idle; /* set init state of Super */
			stateVarCooking = States.LedOff; /* set init state of Cooking */


			//call on entry code of default states
			id = tm.createTimer(Events.evTimeout, CustomTimer.TimerType.SINGLE_SHOT, 0);
			idLed = tm.createTimer(Events.evTimeoutLed, CustomTimer.TimerType.CYCLIC, 500);
			radiator.ovenOff();
			


		}

	}

	protected void reactorChangeToState(States state){
		stateVar=state;
	}

	protected void reactorChangeToStateSuper(States state){
		stateVarSuper = state;
	}
	
	protected void reactorChangeToStateCooking(States state){
		stateVarCooking = state;
	}
	



	public int processEvent(Events msg){

		int evConsumed = 0;

		

		if(m_initialized==false) return 0;

		/* action code */
		// just a comment


		switch (stateVar) {

			case Super:

				switch (stateVarSuper) {

					case Completed:
						if(msg==Events.evDoorOpen){
							/* Transition from Completed to Idle */
							evConsumed=1;

							/* OnEntry code of state Idle */
							radiator.ovenOff();

							/* adjust state variables  */
							stateVarSuper = States.Idle;
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					break; /* end of case Completed  */

					case Cooking:

						switch (stateVarCooking) {

							case LedOn:
								if(msg==Events.evTimeoutLed){
									/* Transition from LedOn to LedOff */
									evConsumed=1;

									/* OnEntry code of state LedOff */
									led.off();

									/* adjust state variables  */
									stateVarCooking = States.LedOff;
								}else{
									/* Intentionally left blank */
								} /*end of event selection */
							break; /* end of case LedOn  */

							case LedOff:
								if(msg==Events.evTimeoutLed){
									/* Transition from LedOff to LedOn */
									evConsumed=1;

									/* OnEntry code of state LedOn */
									led.on();

									/* adjust state variables  */
									stateVarCooking = States.LedOn;
								}else{
									/* Intentionally left blank */
								} /*end of event selection */
							break; /* end of case LedOff  */

							default:
								/* Intentionally left blank */
							break;
						} /* end switch Cooking */

						/* Check if event was already processed  */
						if(evConsumed==0){

							if(msg==Events.evDoorOpen){
								/* Transition from Cooking to CookingPause */
								evConsumed=1;
								
								tm.stop(idLed);

								/* Action code for transition  */
								radiator.ovenOff();
								tm.pause(id);


								/* adjust state variables  */
								stateVarSuper = States.CookingPause;
							}else if(msg==Events.evTimeout){
								/* Transition from Cooking to Completed */
								evConsumed=1;
								
								tm.stop(idLed);

								/* OnEntry code of state Completed */
								radiator.ovenOff();
								tm.stop(id);

								/* adjust state variables  */
								stateVarSuper = States.Completed;
							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						}
					break; /* end of case Cooking  */

					case CookingPause:
						if(msg==Events.evDoorClosed){
							/* Transition from CookingPause to Cooking */
							evConsumed=1;

							/* Action code for transition  */
							tm.cont(id);

							/* OnEntry code of state Cooking */
							radiator.ovenOn();
							tm.start(idLed);
							led.off();
							stateVarSuper = States.Cooking;/* Default in entry chain  */
							stateVarCooking = States.LedOff;/* Default in entry chain  */

						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					break; /* end of case CookingPause  */

					case Idle:
						if(msg==Events.evDoorClosed){
							if(tm.preset(id)>0){
								/* Transition from Idle to Cooking */
								evConsumed=1;

								/* Action code for transition  */
								tm.start(id);

								/* OnEntry code of state Cooking */
								radiator.ovenOn();
								tm.start(idLed);
								led.off();
								stateVarSuper = States.Cooking;/* Default in entry chain  */
								stateVarCooking = States.LedOff;/* Default in entry chain  */

							}else{
								/* Intentionally left blank */
							} /*end of event selection */
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					break; /* end of case Idle  */

					default:
						/* Intentionally left blank */
					break;
				} /* end switch Super */

				/* Check if event was already processed  */
				if(evConsumed==0){

					if(msg==Events.evDec){
						/* Transition from Super to Super */
						evConsumed=1;
						
						if(stateVarSuper== States.Cooking){
							tm.stop(idLed);

						}

						/* Action code for transition  */
						tm.dec(id);

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Completed){
							radiator.ovenOff();
							tm.stop(id);

						}else if(stateVarSuper== States.Cooking){
							radiator.ovenOn();
							tm.start(idLed);

							led.off();
							stateVarSuper = States.Cooking;/* Default in entry chain  */
							stateVarCooking = States.LedOff;/* Default in entry chain  */
						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

					}else if(msg==Events.evInc){
						/* Transition from Super to Super */
						evConsumed=1;
						
						if(stateVarSuper== States.Cooking){
							tm.stop(idLed);

						}

						/* Action code for transition  */
						tm.inc(id);

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Completed){
							radiator.ovenOff();
							tm.stop(id);

						}else if(stateVarSuper== States.Cooking){
							radiator.ovenOn();
							tm.start(idLed);

							led.off();
							stateVarSuper = States.Cooking;/* Default in entry chain  */
							stateVarCooking = States.LedOff;/* Default in entry chain  */
						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

					}else if(msg==Events.evPwrHigh){
						/* Transition from Super to Super */
						evConsumed=1;
						
						if(stateVarSuper== States.Cooking){
							tm.stop(idLed);

						}

						/* Action code for transition  */
						radiator.ovenSetPwrHigh();

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Completed){
							radiator.ovenOff();
							tm.stop(id);

						}else if(stateVarSuper== States.Cooking){
							radiator.ovenOn();
							tm.start(idLed);

							led.off();
							stateVarSuper = States.Cooking;/* Default in entry chain  */
							stateVarCooking = States.LedOff;/* Default in entry chain  */
						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

					}else if(msg==Events.evPwrLow){
						/* Transition from Super to Super */
						evConsumed=1;
						
						if(stateVarSuper== States.Cooking){
							tm.stop(idLed);

						}

						/* Action code for transition  */
						radiator.ovenSetPwrLow();

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Completed){
							radiator.ovenOff();
							tm.stop(id);

						}else if(stateVarSuper== States.Cooking){
							radiator.ovenOn();
							tm.start(idLed);

							led.off();
							stateVarSuper = States.Cooking;/* Default in entry chain  */
							stateVarCooking = States.LedOff;/* Default in entry chain  */
						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				}
			break; /* end of case Super  */

			default:
				/* Intentionally left blank */
			break;
		} /* end switch stateVar_root */
		return evConsumed;
	}
}
