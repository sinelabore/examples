/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -v -l csharp -p ssc -Trace -o Oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Jun 02 17:41:58 CEST 2024 / Version 6.3.3.4114 */




public partial class Oven
{

	public enum States : int{
		Completed,
		Cooking,
		CookingPause,
		Idle,
		Super,
		__UNKNOWN_STATE__
	}

	public enum Events : int {
		evPwrLow,
		evTimeout,
		evDec,
		evDoorOpen,
		evDoorClosed,
		evPwrHigh,
		evInc,
		OVEN_NO_MSG
	}


	// flag if initialized
	protected bool m_initialized=false;


	//state variables
	States stateVar;
	States stateVarSuper;

	// State handler class default ctor
	public Oven()
	{
		m_initialized=false;
	}

	/* Helper(s) to reset history */
	public void ResetHistorySuper(){stateVarSuper=States.Idle;}

	/* Helper to get innermost active state id */
	public States GetInnermostActiveState() {
		if(IsInIdle()){
			return States.Idle;
		}else if(IsInCookingPause()){
			return States.CookingPause;
		}else if(IsInCooking()){
			return States.Cooking;
		}else if(IsInCompleted()){
			return States.Completed;
		}else{
			return States.__UNKNOWN_STATE__;
		}
	}



	// Helper(s) to find out if the machine is in a certain state
	public bool IsInCompleted(){return (((stateVarSuper== States.Completed)&&(stateVar== States.Super)) ? (true) : (false));}
	public bool IsInCooking(){return (((stateVarSuper== States.Cooking)&&(stateVar== States.Super)) ? (true) : (false));}
	public bool IsInCookingPause(){return (((stateVarSuper== States.CookingPause)&&(stateVar== States.Super)) ? (true) : (false));}
	public bool IsInIdle(){return (((stateVarSuper== States.Idle)&&(stateVar== States.Super)) ? (true) : (false));}
	public bool IsInSuper(){return (((stateVar== States.Super)) ? (true) : (false));}


	// overwrite method to add own trace code
	protected virtual void OvenTraceEvent(int evt){
		 Console.WriteLine("Trace Event: " + traceEvents[evt]);
	}

	public String[] traceEvents={
		"evInc",
		"evDec",
		"evPwrHigh",
		"evPwrLow",
		"evDoorOpen",
		"evTimeout",
		"evDoorClosed",
		"evDoorClosed[tm.PresetTimer()>0]"
	};




	public void Initialize(){

		if(m_initialized==false){

			m_initialized=true;


			// Set state vars to default states
			stateVar = States.Super; /* set init state of top state */
			stateVarSuper = States.Idle; /* set init state of Super */


			//call on entry code of default states
			radiator.ovenOff();
			


		}

	}

	protected void ovenChangeToState(States state){
		stateVar=state;
	}

	


	public int ProcessEvent(Events msg){

		int evConsumed = 0;

		

		if(m_initialized==false) return 0;


		switch (stateVar) {

			case States.Super:

				switch (stateVarSuper) {

					case States.Completed:
						if(msg==Events.evDoorOpen){
							/* Transition from Completed to Idle */
							evConsumed=1;

							/* OnEntry code of state Idle */
							radiator.ovenOff();

							/* adjust state variables  */
							stateVarSuper = States.Idle;
							OvenTraceEvent(4);
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					break; /* end of case Completed  */

					case States.Cooking:
						if(msg==Events.evDoorOpen){
							/* Transition from Cooking to CookingPause */
							evConsumed=1;

							/* Action code for transition  */
							radiator.ovenOff();
							tm.PauseTimer();


							/* adjust state variables  */
							stateVarSuper = States.CookingPause;
							OvenTraceEvent(4);
						}else if(msg==Events.evTimeout){
							/* Transition from Cooking to Completed */
							evConsumed=1;

							/* Action code for transition  */
							radiator.ovenOff();
							tm.StopTimer();


							/* adjust state variables  */
							stateVarSuper = States.Completed;
							OvenTraceEvent(5);
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					break; /* end of case Cooking  */

					case States.CookingPause:
						if(msg==Events.evDoorClosed){
							/* Transition from CookingPause to Cooking */
							evConsumed=1;

							/* Action code for transition  */
							tm.ContinueTimer();

							/* OnEntry code of state Cooking */
							radiator.ovenOn();

							/* adjust state variables  */
							stateVarSuper = States.Cooking;
							OvenTraceEvent(6);
						}else{
							/* Intentionally left blank */
						} /*end of event selection */
					break; /* end of case CookingPause  */

					case States.Idle:
						if(msg==Events.evDoorClosed){
							if(tm.PresetTimer()>0){
								/* Transition from Idle to Cooking */
								evConsumed=1;

								/* Action code for transition  */
								tm.StartTimer(false); //singleshot

								/* OnEntry code of state Cooking */
								radiator.ovenOn();

								/* adjust state variables  */
								stateVarSuper = States.Cooking;
								OvenTraceEvent(7);
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
						
						/* Action code for transition  */
						tm.DecrementTimer();

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Cooking){
							radiator.ovenOn();

						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

						OvenTraceEvent(1);
					}else if(msg==Events.evInc){
						/* Transition from Super to Super */
						evConsumed=1;
						
						/* Action code for transition  */
						tm.IncrementTimer();

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Cooking){
							radiator.ovenOn();

						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

						OvenTraceEvent(0);
					}else if(msg==Events.evPwrHigh){
						/* Transition from Super to Super */
						evConsumed=1;
						
						/* Action code for transition  */
						radiator.ovenSetPwrHigh();

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Cooking){
							radiator.ovenOn();

						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

						OvenTraceEvent(2);
					}else if(msg==Events.evPwrLow){
						/* Transition from Super to Super */
						evConsumed=1;
						
						/* Action code for transition  */
						radiator.ovenSetPwrLow();

						stateVar = States.Super;/* entry chain  */
						if(stateVarSuper== States.Cooking){
							radiator.ovenOn();

						}else if(stateVarSuper== States.Idle){
							radiator.ovenOff();

						}

						OvenTraceEvent(3);
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
