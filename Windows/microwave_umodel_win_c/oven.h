/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p UModel -l cx -t MicrowaveOven:MachineClass -doxygen -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually*/
/* Generated on: Fri Aug 25 08:41:12 CEST 2023 / Version 6.0.5.3690 */


#ifndef OVEN_H
#define OVEN_H


/* Header to define simple data types */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	Active,
	Completed,
	Cooking,
	CookingPause,
	Error,
	Idle,
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
typedef struct{
	OVEN_ENTRY_FLAG_T activeEntry;
	OVEN_STATEVAR_T stateVar;
	OVEN_STATEVAR_T stateVarActive;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	OVEN_INST_ID_T inst_id;
} OVEN_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Active /* set init state of top state */,\
	(OVEN_STATEVAR_T) Idle /* set init state of Active */,\
	0 /* instance id */ } 


/* Helper(s) to reset history */
#define OVEN_RESET_HISTORY_ACTIVE(instance) ((&(instance))->stateVarActive=Idle)

/*Prototype of the state machine function */
void  oven(OVEN_INSTANCEDATA_T *instanceVar);


/*Prototoye of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
void ovenResetHistoryActive(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t ovenIsInActive(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInError(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_ACTIVE(instance)((((&instance)->stateVar==Active)) ? (1U) : (0U))
#define OVEN_IS_IN_COMPLETED(instance)((((&instance)->stateVarActive==Completed)&&((&instance)->stateVar==Active)) ? (1U) : (0U))
#define OVEN_IS_IN_ERROR(instance)((((&instance)->stateVar==Error)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKING(instance)((((&instance)->stateVarActive==Cooking)&&((&instance)->stateVar==Active)) ? (1U) : (0U))
#define OVEN_IS_IN_IDLE(instance)((((&instance)->stateVarActive==Idle)&&((&instance)->stateVar==Active)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKINGPAUSE(instance)((((&instance)->stateVarActive==CookingPause)&&((&instance)->stateVar==Active)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar);

#endif
