/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options:   */
/* This file is generated from sm.xml - do not edit manually*/
/* Generated on: Sun Oct 08 21:03:24 CEST 2023 / Version 6.1.3705 */


#ifndef SM_H
#define SM_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	Failsafe,
	Off,
	On,
	Operational,
	__SM_LAST_STATE__} SM_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef PROVIDE_OWN_SM_STATEMACHINE_TYPES
	typedef uint8_t SM_ENTRY_FLAG_T;
	typedef SM_STATES_T SM_STATEVAR_T;
	typedef uint8_t SM_INST_ID_T;
	typedef uint8_t SM_EV_CONSUMED_FLAG_T ;
#endif

/* Instance data definition of state machine */
typedef struct {
	SM_ENTRY_FLAG_T operationalEntry;
	SM_STATEVAR_T stateVar;
	SM_STATEVAR_T stateVarOperational;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	SM_INST_ID_T inst_id;
} SM_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define SM_INSTANCEDATA_INIT {\
	1,\
	(SM_STATEVAR_T) Operational /* set init state of top state */,\
	(SM_STATEVAR_T) Off /* set init state of Operational */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void smInitMachine(SM_INSTANCEDATA_T * const instanceVar, SM_INST_ID_T instId);

/* Helper(s) to reset history */
#define SM_RESET_HISTORY_OPERATIONAL(instance) ((&(instance))->stateVarOperational=Off)

/* Prototype of the state machine function */
void  sm(SM_INSTANCEDATA_T *instanceVar, SM_EVENT_T  msg);


/* Prototoye of the reset machine function */
void smResetMachine(SM_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
void smResetHistoryOperational(SM_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t smIsInFailsafe(const SM_INSTANCEDATA_T * const instanceVar);
uint8_t smIsInOff(const SM_INSTANCEDATA_T * const instanceVar);
uint8_t smIsInOn(const SM_INSTANCEDATA_T * const instanceVar);
uint8_t smIsInOperational(const SM_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define SM_IS_IN_FAILSAFE(instance)((((&instance)->stateVar==Failsafe)) ? (1U) : (0U))
#define SM_IS_IN_OFF(instance)((((&instance)->stateVarOperational==Off)&&((&instance)->stateVar==Operational)) ? (1U) : (0U))
#define SM_IS_IN_ON(instance)((((&instance)->stateVarOperational==On)&&((&instance)->stateVar==Operational)) ? (1U) : (0U))
#define SM_IS_IN_OPERATIONAL(instance)((((&instance)->stateVar==Operational)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
SM_STATES_T smGetInnermostActiveState(const SM_INSTANCEDATA_T * const instanceVar);


#endif
