/*
 * (c) Peter Mueller, 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -t Model:Class Model:decoder -p EA -o decoder decoder.xml   */
/* This file is generated from decoder.xml - do not edit manually  */
/* Generated on: Wed Dec 28 17:29:00 CET 2022 / Version 5.5.5.6 */



#include "mydefs.h"
#include "decoder_ext.h"
#include "decoder.h"

TUSIGN8 invalidFlag, secCnt, bitpos;
TUSIGN8 minute,  hour, day,dayOfWeek, month, year;
DECODER_INSTANCEDATA_T instanceVar=DECODER_INSTANCEDATA_INIT;

extern TUSIGN8 msg;
extern TUSIGN8 len;

extern TUSIGN8 setClock(void);

void decoderChangeToState(DECODER_STATES_T state){
  instanceVar.stateVar=state;
}

void decoderChangeToStateMinDetected(DECODER_STATES_T state){
  instanceVar.stateVarMinDetected=state;
}

TUSIGN8 update(TUSIGN8 in){
  if(len==LONG) in=in|bitpos;
  bitpos=bitpos<<1;
  return in;
}

void setTimeToZero(void){
  minute=0;
  hour=0;
  day=0;
  dayOfWeek=0;
  month=0;
  year=0;
}





void  decoder(void){

	DECODER_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar.waitforminstartEntry==1U){
		invalidFlag=1;
		

		instanceVar.waitforminstartEntry=0U;
	}


	switch (instanceVar.stateVar) {

		case MinDetected:

			switch (instanceVar.stateVarMinDetected) {

				case Day:
					if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt<=41){
							/* Transition from Day to Day */
							evConsumed=1U;

							/* Action code for transition  */
							day=update(day);
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Day;
						}else{
							/* Transition from Day to DayOfWeek */
							evConsumed=1U;

							/* Action code for transition  */
							day=update(day);
							secCnt++;
							bitpos=1;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = DayOfWeek;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Day  */

				case DayOfWeek:
					if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt<=44){
							/* Transition from DayOfWeek to DayOfWeek */
							evConsumed=1U;

							/* Action code for transition  */
							dayOfWeek=update(dayOfWeek);
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = DayOfWeek;
						}else{
							/* Transition from DayOfWeek to Month */
							evConsumed=1U;

							/* Action code for transition  */
							dayOfWeek=update(dayOfWeek);
							secCnt++;
							bitpos=1;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Month;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case DayOfWeek  */

				case Hour:
					if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt<=35){
							/* Transition from Hour to Hour */
							evConsumed=1U;

							/* Action code for transition  */
							hour=update(hour);
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Hour;
						}else{
							/* Transition from Hour to Day */
							evConsumed=1U;

							/* Action code for transition  */
							hour=update(hour);
							secCnt++;
							bitpos=1;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Day;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Hour  */

				case IgnoreBBKMeteo:
					if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt==21){
							/* Transition from IgnoreBBKMeteo to Minute */
							evConsumed=1U;

							/* Action code for transition  */
							secCnt++;
							bitpos=1;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Minute;
						}else{
							/* Transition from IgnoreBBKMeteo to IgnoreBBKMeteo */
							evConsumed=1U;

							/* Action code for transition  */
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = IgnoreBBKMeteo;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case IgnoreBBKMeteo  */

				case Minute:
					if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt<=28){
							/* Transition from Minute to Minute */
							evConsumed=1U;

							/* Action code for transition  */
							minute = update(minute);
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Minute;
						}else{
							/* Transition from Minute to Hour */
							evConsumed=1U;

							/* Action code for transition  */
							minute=update(minute);
							secCnt++;
							bitpos=1;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Hour;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Minute  */

				case Month:
					if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt<=49){
							/* Transition from Month to Month */
							evConsumed=1U;

							/* Action code for transition  */
							month=update(month);
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Month;
						}else{
							/* Transition from Month to Year */
							evConsumed=1U;

							/* Action code for transition  */
							month=update(month);
							secCnt++;
							bitpos=1;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Year;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Month  */

				case Year:
					if(msg==(DECODER_EVENT_T)evMinStart){
						/* Transition from Year to IgnoreBBKMeteo */
						evConsumed=1U;

						/* Action code for transition  */
						invalidFlag = setClock();
						setTimeToZero();
						secCnt=0x1;



						/* adjust state variables  */
						instanceVar.stateVarMinDetected = IgnoreBBKMeteo;
					}else if(msg==(DECODER_EVENT_T)evTick){
						if(secCnt<=59){
							/* Transition from Year to Year */
							evConsumed=1U;

							/* Action code for transition  */
							year=update(year);
							secCnt++;



							/* adjust state variables  */
							instanceVar.stateVarMinDetected = Year;
						}else{
							/* Transition from Year to WaitForMinStart */
							evConsumed=1U;


							/* OnEntry code of state WaitForMinStart */
							invalidFlag=1;

							/* adjust state variables  */
							instanceVar.stateVar = WaitForMinStart;
							instanceVar.stateVarMinDetected = IgnoreBBKMeteo;
						} /*end of event selection */
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Year  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch MinDetected */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(DECODER_EVENT_T)evTimingError){
					/* Transition from MinDetected to WaitForMinStart */
					evConsumed=1U;
					
					/* OnEntry code of state WaitForMinStart */
					invalidFlag=1;

					/* adjust state variables  */
					instanceVar.stateVar = WaitForMinStart;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case MinDetected  */

		case WaitForMinStart:
			if(msg==(DECODER_EVENT_T)evMinStart){
				/* Transition from WaitForMinStart to MinDetected */
				evConsumed=1U;

				/* Action code for transition  */
				secCnt=0x1;
				setTimeToZero();


				instanceVar.stateVar = MinDetected;/* Default in entry chain  */
				instanceVar.stateVarMinDetected = IgnoreBBKMeteo;/* Default in entry chain  */

			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case WaitForMinStart  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void decoderResetMachine(void){
	instanceVar.waitforminstartEntry = 1U;
	instanceVar.stateVar = WaitForMinStart;
	instanceVar.stateVarMinDetected = IgnoreBBKMeteo;

}
// Helper(s) to reset history
void decoderResetHistoryMinDetected(void){
	instanceVar.stateVarMinDetected=IgnoreBBKMeteo;
}


// Helper(s) to find out if the machine is in a certain state

uint8_t decoderIsInWaitForMinStart(void){
	return(((instanceVar.stateVar== WaitForMinStart)) ? (1U) : (0U));
}

uint8_t decoderIsInYear(void){
	return(((instanceVar.stateVarMinDetected== Year)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInMinDetected(void){
	return(((instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInMinute(void){
	return(((instanceVar.stateVarMinDetected== Minute)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInMonth(void){
	return(((instanceVar.stateVarMinDetected== Month)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInDay(void){
	return(((instanceVar.stateVarMinDetected== Day)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInIgnoreBBKMeteo(void){
	return(((instanceVar.stateVarMinDetected== IgnoreBBKMeteo)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInDayOfWeek(void){
	return(((instanceVar.stateVarMinDetected== DayOfWeek)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}

uint8_t decoderIsInHour(void){
	return(((instanceVar.stateVarMinDetected== Hour)&&(instanceVar.stateVar== MinDetected)) ? (1U) : (0U));
}


// Helper to get id of innermost active state
DECODER_STATES_T decoderGetInnermostActiveState(void){
	DECODER_STATES_T innermost;

	if(decoderIsInHour()){
		innermost =Hour;
	}else if(decoderIsInDayOfWeek()){
		innermost = DayOfWeek;
	}else if(decoderIsInIgnoreBBKMeteo()){
		innermost = IgnoreBBKMeteo;
	}else if(decoderIsInDay()){
		innermost = Day;
	}else if(decoderIsInMonth()){
		innermost = Month;
	}else if(decoderIsInMinute()){
		innermost = Minute;
	}else if(decoderIsInYear()){
		innermost = Year;
	}else if(decoderIsInWaitForMinStart()){
		innermost = WaitForMinStart;
	}else{
		innermost = __DECODER_LAST_STATE__;
	}
	return innermost;}

