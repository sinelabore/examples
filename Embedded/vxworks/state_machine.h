/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -E -p ssc   */
/* This file is generated from state_machine.xml - do not edit manually*/
/* Generated on: Tue Mar 07 15:02:52 CET 2023 / Version 5.5.6.3515 */


#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


/* Header to define simple data types */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	Fast,
	Final0,
	RUN,
	Slow,
	STOP,
	__STATE_MACHINE_LAST_STATE__} STATE_MACHINE_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef PROVIDE_OWN_STATE_MACHINE_STATEMACHINE_TYPES
	typedef uint8_t STATE_MACHINE_ENTRY_FLAG_T;
	typedef STATE_MACHINE_STATES_T STATE_MACHINE_STATEVAR_T;
	typedef uint8_t STATE_MACHINE_INST_ID_T;
	typedef uint8_t STATE_MACHINE_EV_CONSUMED_FLAG_T ;
#endif




/* Instance data definition of state machine */
typedef struct{
	STATE_MACHINE_ENTRY_FLAG_T stopEntry;
	STATE_MACHINE_STATEVAR_T stateVar;
	STATE_MACHINE_STATEVAR_T stateVarRUN;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	STATE_MACHINE_INST_ID_T inst_id;
} STATE_MACHINE_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define STATE_MACHINE_INSTANCEDATA_INIT {\
	1,\
	(STATE_MACHINE_STATEVAR_T) STOP /* set init state of top state */,\
	(STATE_MACHINE_STATEVAR_T) Slow /* set init state of RUN */,\
	0 /* instance id */ } 


/* Helper(s) to reset history */
#define STATE_MACHINE_RESET_HISTORY_RUN(instance) ((&(instance))->stateVarRUN=Slow)

/*Prototype of the state machine function */
void  state_machine(STATE_MACHINE_INSTANCEDATA_T *instanceVar, STATE_MACHINE_EVENT_T  msg);


/*Prototoye of the reset machine function */
void state_machineResetMachine(STATE_MACHINE_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
void state_machineResetHistoryRUN(STATE_MACHINE_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t state_machineIsInFast(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar);
uint8_t state_machineIsInFinal0(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar);
uint8_t state_machineIsInRUN(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar);
uint8_t state_machineIsInSTOP(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar);
uint8_t state_machineIsInSlow(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define STATE_MACHINE_IS_IN_FAST(instance)((((&instance)->stateVarRUN==Fast)&&((&instance)->stateVar==RUN)) ? (1U) : (0U))
#define STATE_MACHINE_IS_IN_FINAL0(instance)((((&instance)->stateVar==Final0)) ? (1U) : (0U))
#define STATE_MACHINE_IS_IN_RUN(instance)((((&instance)->stateVar==RUN)) ? (1U) : (0U))
#define STATE_MACHINE_IS_IN_STOP(instance)((((&instance)->stateVar==STOP)) ? (1U) : (0U))
#define STATE_MACHINE_IS_IN_SLOW(instance)((((&instance)->stateVarRUN==Slow)&&((&instance)->stateVar==RUN)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
STATE_MACHINE_STATES_T state_machineGetInnermostActiveState(const STATE_MACHINE_INSTANCEDATA_T * const instanceVar);

#endif
