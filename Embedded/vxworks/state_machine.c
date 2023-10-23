/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -E -p ssc   */
/* This file is generated from state_machine.xml - do not edit manually  */
/* Generated on: Tue Mar 07 15:02:52 CET 2023 / Version 5.5.6.3515 */


#include <stdio.h>
#include <stdint.h>
#include <vxWorks.h>

#include "state_machine_ext.h"
#include "state_machine.h"

void startTimer(void);
void stopTimer(void);




void  state_machine(STATE_MACHINE_INSTANCEDATA_T *instanceVar, STATE_MACHINE_EVENT_T  msg){

	STATE_MACHINE_EV_CONSUMED_FLAG_T evConsumed = 0U;

	
	/*execute entry code of default state once to init machine */
	if(instanceVar->stopEntry==1U){
		printf("Enter STOP\n");
		

		instanceVar->stopEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case RUN:
			/* action code  */
			printf("In RUN\n");



			switch (instanceVar->stateVarRUN) {

				case Fast:
					if(msg==(STATE_MACHINE_EVENT_T)evSlow){
						/* Transition from Fast to Slow */
						evConsumed=1U;
						/* OnExit code of state Fast */
						printf("Exit Fast\n");

						/* OnEntry code of state Slow */
						printf("Enter Slow\n");

						/* adjust state variables  */
						instanceVar->stateVarRUN = Slow;
					}else if(msg==(STATE_MACHINE_EVENT_T)evTimeout){
						/* Transition from Fast to Slow */
						evConsumed=1U;
						/* OnExit code of state Fast */
						printf("Exit Fast\n");

						/* OnEntry code of state Slow */
						printf("Enter Slow\n");

						/* adjust state variables  */
						instanceVar->stateVarRUN = Slow;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Fast  */

				case Slow:
					if(msg==(STATE_MACHINE_EVENT_T)evFast){
						/* Transition from Slow to Fast */
						evConsumed=1U;
						/* OnExit code of state Slow */
						printf("Exit Slow\n");

						/* OnEntry code of state Fast */
						printf("Enter Fast\n");

						/* adjust state variables  */
						instanceVar->stateVarRUN = Fast;
					}else if(msg==(STATE_MACHINE_EVENT_T)evTimeout){
						/* Transition from Slow to Fast */
						evConsumed=1U;
						/* OnExit code of state Slow */
						printf("Exit Slow\n");

						/* OnEntry code of state Fast */
						printf("Enter Fast\n");

						/* adjust state variables  */
						instanceVar->stateVarRUN = Fast;
					}else{
						/* Intentionally left blank */
					} /*end of event selection */
				break; /* end of case Slow  */

				default:
					/* Intentionally left blank */
				break;
			} /* end switch RUN */

			/* Check if event was already processed  */
			if(evConsumed==0U){

				if(msg==(STATE_MACHINE_EVENT_T)evStop){
					/* Transition from RUN to STOP */
					evConsumed=1U;
					
					if(instanceVar->stateVarRUN==Fast){
						printf("Exit Fast\n");


					}else if(instanceVar->stateVarRUN==Slow){
						printf("Exit Slow\n");

					}else {
						/* Intentionally left blank */
					};


					printf("Exit RUN\n");

					/* Action code for transition  */
					stopTimer();


					/* OnEntry code of state STOP */
					printf("Enter STOP\n");

					/* adjust state variables  */
					instanceVar->stateVar = STOP;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}
		break; /* end of case RUN  */

		case STOP:
			/* action code  */
			printf("In STOP\n");


			if(msg==(STATE_MACHINE_EVENT_T)evShutdown){
				/* Transition from STOP to Final0 */
				evConsumed=1U;
				/* OnExit code of state STOP */
				printf("Exit STOP\n");

				/* Action code for transition  */
				printf("Entering Final\n");



				/* adjust state variables  */
				instanceVar->stateVar = Final0;
			}else if(msg==(STATE_MACHINE_EVENT_T)evStart){
				/* Transition from STOP to RUN */
				evConsumed=1U;
				/* OnExit code of state STOP */
				printf("Exit STOP\n");

				/* Action code for transition  */
				startTimer();


				/* OnEntry code of state RUN */
				printf("Enter RUN\n");
				instanceVar->stateVar = RUN;/* entry chain  */
				if(instanceVar->stateVarRUN==Fast){
					printf("Enter Fast\n");

				}else if(instanceVar->stateVarRUN==Slow){
					printf("Enter Slow\n");

				}else {
					/* Intentionally left blank */
				};


			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case STOP  */

		case Final0:
		break; /* end of case Final0  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void state_machineResetMachine(STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	instanceVar->stopEntry = 1U;
	instanceVar->stateVar = STOP;
	instanceVar->stateVarRUN = Slow;

}
// Helper(s) to reset history
void state_machineResetHistoryRUN(STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	instanceVar->stateVarRUN=Slow;
}


// Helper(s) to find out if the machine is in a certain state

uint8_t state_machineIsInFast(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarRUN== Fast)&&(instanceVar->stateVar== RUN)) ? (1U) : (0U));
}

uint8_t state_machineIsInFinal0(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== Final0)) ? (1U) : (0U));
}

uint8_t state_machineIsInRUN(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== RUN)) ? (1U) : (0U));
}

uint8_t state_machineIsInSTOP(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== STOP)) ? (1U) : (0U));
}

uint8_t state_machineIsInSlow(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVarRUN== Slow)&&(instanceVar->stateVar== RUN)) ? (1U) : (0U));
}


// Helper to get id of innermost active state
STATE_MACHINE_STATES_T state_machineGetInnermostActiveState(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar){
	STATE_MACHINE_STATES_T innermost;

	if(state_machineIsInSlow(instanceVar)){
		innermost =Slow;
	}else if(state_machineIsInFast(instanceVar)){
		innermost = Fast;
	}else if(state_machineIsInSTOP(instanceVar)){
		innermost = STOP;
	}else if(state_machineIsInFinal0(instanceVar)){
		innermost = Final0;
	}else{
		innermost = __STATE_MACHINE_LAST_STATE__;
	}
	return innermost;}

