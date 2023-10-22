/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -t Model:Package:oven -p Papyrus -o oven oven.uml   */
/* This file is generated from oven.uml - do not edit manually*/
/* Generated on: Sun Oct 22 18:20:54 CEST 2023 / Version 6.1.3706 */


#ifndef OVEN_H
#define OVEN_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	Completed,
	Cooking,
	CookingPause,
	Idle,
	Super,
	__OVEN_LAST_STATE__} OVEN_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef PROVIDE_OWN_OVEN_STATEMACHINE_TYPES
	typedef uint8_t OVEN_ENTRY_FLAG_T;
	typedef OVEN_STATES_T OVEN_STATEVAR_T;
	typedef uint8_t OVEN_INST_ID_T;
	typedef uint8_t OVEN_EV_CONSUMED_FLAG_T ;
#endif

/* Instance data definition of state machine */
typedef struct {
	OVEN_ENTRY_FLAG_T superEntry;
	OVEN_STATEVAR_T stateVar;
	OVEN_STATEVAR_T stateVarSuper;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	OVEN_INST_ID_T inst_id;
} OVEN_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Super /* set init state of top state */,\
	(OVEN_STATEVAR_T) Idle /* set init state of Super */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId);

/* Helper(s) to reset history */
#define OVEN_RESET_HISTORY_SUPER(instance) ((&(instance))->stateVarSuper=Idle)

/* Prototype of the state machine function */
void  oven(OVEN_INSTANCEDATA_T *instanceVar);


/* Prototoye of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
void ovenResetHistorySuper(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t ovenIsInSuper(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COMPLETED(instance)((((&instance)->stateVarSuper==Completed)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_IDLE(instance)((((&instance)->stateVarSuper==Idle)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKING(instance)((((&instance)->stateVarSuper==Cooking)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKINGPAUSE(instance)((((&instance)->stateVarSuper==CookingPause)&&((&instance)->stateVar==Super)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar);


#endif
