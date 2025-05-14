/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -Trace -p CADIFRA -o complex complex.cdd   */
/* This file is generated from complex.cdd - do not edit manually  */
/* Generated on: Wed May 14 21:43:07 CEST 2025 / 6.5.3 */


#include "complex_ext.h"
#include "complex.h"
#include "complex_trace.h"
#include <stdio.h>





void  complex(COMPLEX_INSTANCEDATA_T *instanceVar, COMPLEX_EVENT_T  msg){

	COMPLEX_EV_CONSUMED_FLAG_T  evConsumed = 0U;


	
	/*execute entry code of default state once to init machine */
	if(instanceVar->s1Entry==1U) {

		complexTraceEvent(instanceVar->inst_id, 17U);

		printf("S12Entry\n");
		

		instanceVar->s1Entry=0U;
	}


	/* action code */
	printf("Outer test action\n");


	switch (instanceVar->stateVar) {

		case S1:

			switch (instanceVar->stateVarS1) {

				case S11:
					/* action code  */
					printf("S11 Action\n");


					if(msg==(COMPLEX_EVENT_T)e1){
						/* Transition from S11 to S11 */
						evConsumed=1U;
						/* OnExit code of state S11 */
						printf("S11Exit\n");

						/* Action code for transition  */
						printf("e1Act\n");


						/* OnEntry code of state S11 */
						printf("S11Entry\n");

						/* adjust state variables  */
						instanceVar->stateVarS1 = S11;
						complexTraceEvent(instanceVar->inst_id, 8U);
					}else if(msg==(COMPLEX_EVENT_T)e12){
						/* Transition from S11 to S12 */
						evConsumed=1U;
						/* OnExit code of state S11 */
						printf("S11Exit\n");

						/* Action code for transition  */
						printf("e12\n");


						/* OnEntry code of state S12 */
						printf("S12Entry\n");

						/* adjust state variables  */
						instanceVar->stateVarS1 = S12;
						complexTraceEvent(instanceVar->inst_id, 12U);
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case S11  */

				case S12:
					/* action code  */
					printf("S12 Action\n");


					if(msg==(COMPLEX_EVENT_T)e13){
						/* Transition from S12 to S22 */
						evConsumed=1U;
						/* OnExit code of state S12 */
						printf("S12Exit\n");


						/* Action code for transition  */
						printf("e13Act\n");



						/* OnEntry code of state S22 */
						printf("S22Entry\n");

						/* adjust state variables  */
						instanceVar->stateVar = S2;
						instanceVar->stateVarS1 = S12;
						instanceVar->stateVarS2 = S22;
						complexTraceEvent(instanceVar->inst_id, 13U);
					}else if(msg==(COMPLEX_EVENT_T)e14){
						/* Transition from S12 to S11 */
						evConsumed=1U;
						/* OnExit code of state S12 */
						printf("S12Exit\n");

						/* Action code for transition  */
						printf("e14\n");


						/* OnEntry code of state S11 */
						printf("S11Entry\n");

						/* adjust state variables  */
						instanceVar->stateVarS1 = S11;
						complexTraceEvent(instanceVar->inst_id, 14U);
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case S12  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch S1 */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(COMPLEX_EVENT_T)e1){
					/* Transition from S1 to S2 */
					evConsumed=1U;
					
					if(instanceVar->stateVarS1==S11){
						printf("S11Exit\n");


					}else if(instanceVar->stateVarS1==S12){
						printf("S12Exit\n");

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition  */
					printf("e1Act\n");


					instanceVar->stateVar = S2;/* entry chain  */
					if(instanceVar->stateVarS2==S21){
						printf("S21Entry\n");

					}else if(instanceVar->stateVarS2==S22){
						printf("S22Entry\n");

					}else {
						/* Intentionally left blank */
					};


					/* adjust state variables  */
					instanceVar->stateVarS1 = S12;
					complexTraceEvent(instanceVar->inst_id, 8U);
				}else if(msg==(COMPLEX_EVENT_T)e2){
					/* Transition from S1 to S22 */
					evConsumed=1U;
					
					if(instanceVar->stateVarS1==S11){
						printf("S11Exit\n");


					}else if(instanceVar->stateVarS1==S12){
						printf("S12Exit\n");

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition  */
					printf("e2Act\n");



					/* OnEntry code of state S22 */
					printf("S22Entry\n");

					/* adjust state variables  */
					instanceVar->stateVar = S2;
					instanceVar->stateVarS1 = S12;
					instanceVar->stateVarS2 = S22;
					complexTraceEvent(instanceVar->inst_id, 0U);
				}else if(msg==(COMPLEX_EVENT_T)e4){
					/* Transition from S1 to S1 */
					evConsumed=1U;
					
					if(instanceVar->stateVarS1==S11){
						printf("S11Exit\n");


					}else if(instanceVar->stateVarS1==S12){
						printf("S12Exit\n");

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition  */
					printf("e4Act\n");


					printf("S12Entry\n");
					instanceVar->stateVar = S1; /* Default in entry chain  */
					instanceVar->stateVarS1 = S12; /* Default in entry chain  */

					complexTraceEvent(instanceVar->inst_id, 3U);
				}else if(msg==(COMPLEX_EVENT_T)e5){
					/* Transition from S1 to S11 */
					evConsumed=1U;
					
					if(instanceVar->stateVarS1==S11){
						printf("S11Exit\n");


					}else if(instanceVar->stateVarS1==S12){
						printf("S12Exit\n");

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition  */
					printf("e5Act\n");



					/* OnEntry code of state S11 */
					printf("S11Entry\n");

					/* adjust state variables  */
					instanceVar->stateVar = S1;
					instanceVar->stateVarS1 = S11;
					complexTraceEvent(instanceVar->inst_id, 4U);
				}else if(msg==(COMPLEX_EVENT_T)e7){
					/* Transition from S1 to S3 */
					evConsumed=1U;
					
					if(instanceVar->stateVarS1==S11){
						printf("S11Exit\n");


					}else if(instanceVar->stateVarS1==S12){
						printf("S12Exit\n");

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition  */
					printf("e7Act\n");


					/* OnEntry code of state S3 */
					printf("S3Entry\n");

					/* adjust state variables  */
					instanceVar->stateVar = S3;
					instanceVar->stateVarS1 = S12;
					complexTraceEvent(instanceVar->inst_id, 6U);
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case S1  */

		case S2:

			switch (instanceVar->stateVarS2) {

				case S21:
					/* action code  */
					printf("S21 Action\n");


					if(msg==(COMPLEX_EVENT_T)e16){
						/* Transition from S21 to S22 */
						evConsumed=1U;
						/* OnExit code of state S21 */
						printf("S21Exit\n");

						/* Action code for transition  */
						printf("e16\n");


						/* OnEntry code of state S22 */
						printf("S22Entry\n");

						/* adjust state variables  */
						instanceVar->stateVarS2 = S22;
						complexTraceEvent(instanceVar->inst_id, 16U);
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case S21  */

				case S22:
					/* action code  */
					printf("S22 Action\n");


					if(msg==(COMPLEX_EVENT_T)e15){
						/* Transition from S22 to S21 */
						evConsumed=1U;
						/* OnExit code of state S22 */
						printf("S22Exit\n");

						/* Action code for transition  */
						printf("e15\n");


						/* OnEntry code of state S21 */
						printf("S21Entry\n");

						/* adjust state variables  */
						instanceVar->stateVarS2 = S21;
						complexTraceEvent(instanceVar->inst_id, 15U);
					}else if(msg==(COMPLEX_EVENT_T)e20){
						evConsumed=1U;
						/* Action code for inner transition  */
						printf("e20Act\n");


						complexTraceEvent(instanceVar->inst_id, 2U);
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case S22  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch S2 */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(COMPLEX_EVENT_T)e6){
					/* Transition from S2 to S1 */
					evConsumed=1U;
					
					if(instanceVar->stateVarS2==S21){
						printf("S21Exit\n");


					}else if(instanceVar->stateVarS2==S22){
						printf("S22Exit\n");

					}else {
						/* Intentionally left blank */
					};


					/* Action code for transition  */
					printf("e6Act\n");


					printf("S12Entry\n");
					instanceVar->stateVar = S1; /* Default in entry chain  */
					instanceVar->stateVarS1 = S12; /* Default in entry chain  */

					complexTraceEvent(instanceVar->inst_id, 1U);
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case S2  */

		case S3:
			/* action code  */
			printf("S3 Action\n");


			if(msg==(COMPLEX_EVENT_T)e21){
				evConsumed=1U;
				/* Action code for inner transition  */
				printf("e21Act\n");


				complexTraceEvent(instanceVar->inst_id, 5U);
			}else if(msg==(COMPLEX_EVENT_T)e8){
				/* Transition from S3 to S3 */
				evConsumed=1U;
				/* OnExit code of state S3 */
				printf("S3Exit\n");

				/* Action code for transition  */
				printf("e8Act\n");


				/* OnEntry code of state S3 */
				printf("S3Entry\n");

				/* adjust state variables  */
				instanceVar->stateVar = S3;
				complexTraceEvent(instanceVar->inst_id, 7U);
			}else if(msg==(COMPLEX_EVENT_T)e9){
				/* Transition from S3 to S4 */
				evConsumed=1U;
				/* OnExit code of state S3 */
				printf("S3Exit\n");

				/* Action code for transition  */
				printf("e9Act\n");


				/* OnEntry code of state S4 */
				printf("S4Entry\n");

				/* adjust state variables  */
				instanceVar->stateVar = S4;
				complexTraceEvent(instanceVar->inst_id, 9U);
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case S3  */

		case S4:
			/* action code  */
			printf("S4 Action\n");


			if(msg==(COMPLEX_EVENT_T)e10){
				/* Transition from S4 to S1 */
				evConsumed=1U;
				/* OnExit code of state S4 */
				printf("S4Exit\n");

				/* Action code for transition  */
				printf("e10Act\n");


				printf("S12Entry\n");
				instanceVar->stateVar = S1; /* Default in entry chain  */
				instanceVar->stateVarS1 = S12; /* Default in entry chain  */

				complexTraceEvent(instanceVar->inst_id, 10U);
			}else if(msg==(COMPLEX_EVENT_T)e11){
				/* Transition from S4 to S12 */
				evConsumed=1U;
				/* OnExit code of state S4 */
				printf("S4Exit\n");

				/* Action code for transition  */
				printf("e11Act\n");



				/* OnEntry code of state S12 */
				printf("S12Entry\n");

				/* adjust state variables  */
				instanceVar->stateVar = S1;
				instanceVar->stateVarS1 = S12;
				complexTraceEvent(instanceVar->inst_id, 11U);
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case S4  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Initialize state machine */
void complexInitMachine(COMPLEX_INSTANCEDATA_T * const instanceVar, COMPLEX_INST_ID_T instId) {
	instanceVar->s1Entry = 1U;
	instanceVar->stateVar = S1;
	instanceVar->stateVarS1 = S12;
	instanceVar->stateVarS2 = S21;

	instanceVar->inst_id = instId;

}

/* Implementation of the reset machine function */
void complexResetMachine(COMPLEX_INSTANCEDATA_T * const instanceVar) {
	instanceVar->s1Entry = 1U;
	instanceVar->stateVar = S1;
	instanceVar->stateVarS1 = S12;
	instanceVar->stateVarS2 = S21;

}
/* Helper(s) to reset history */
void complexResetHistoryS1(COMPLEX_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarS1=S12;
}
void complexResetHistoryS2(COMPLEX_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarS2=S21;
}

/* Helper(s) to find out if the machine is in a certain state */

uint8_t complexIsInS2(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== S2)) ? (1U) : (0U));
}

uint8_t complexIsInS21(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarS2== S21)&&(instanceVar->stateVar== S2)) ? (1U) : (0U));
}

uint8_t complexIsInS22(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarS2== S22)&&(instanceVar->stateVar== S2)) ? (1U) : (0U));
}

uint8_t complexIsInS1(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== S1)) ? (1U) : (0U));
}

uint8_t complexIsInS11(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarS1== S11)&&(instanceVar->stateVar== S1)) ? (1U) : (0U));
}

uint8_t complexIsInS12(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVarS1== S12)&&(instanceVar->stateVar== S1)) ? (1U) : (0U));
}

uint8_t complexIsInS3(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== S3)) ? (1U) : (0U));
}

uint8_t complexIsInS4(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
	return(((instanceVar->stateVar== S4)) ? (1U) : (0U));
}


/* Helper to get id of innermost active state */
COMPLEX_STATES_T complexGetInnermostActiveState(const COMPLEX_INSTANCEDATA_T * const instanceVar) {
    	COMPLEX_STATES_T innermost;

	if(complexIsInS12(instanceVar)) {
    		innermost = S12;
	} else if(complexIsInS11(instanceVar)) {
    		innermost = S11;
	} else if(complexIsInS22(instanceVar)) {
    		innermost = S22;
	} else if(complexIsInS21(instanceVar)) {
    		innermost = S21;
	} else if(complexIsInS4(instanceVar)) {
    		innermost = S4;
	} else if(complexIsInS3(instanceVar)) {
    		innermost = S3;
	} else {
    		innermost = __COMPLEX_LAST_STATE__;
	}
	return innermost;
}


