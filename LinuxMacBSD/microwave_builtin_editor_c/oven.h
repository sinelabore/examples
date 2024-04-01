/* (C) Your company *//* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually*/
/* Generated on: Fri Mar 15 11:23:55 CET 2024 / Version 6.3.2.3814 */


#ifndef OVEN_H
#define OVEN_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>

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
	OVEN_STATEVAR_T stateVarMainRegion;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	OVEN_INST_ID_T inst_id;
} OVEN_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Super /* set init state of top state */,\
	(OVEN_STATEVAR_T) Idle /* set init state of MainRegion */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId);


/* Prototype of the state machine function */
void  oven(OVEN_INSTANCEDATA_T *instanceVar, OVEN_EVENT_T  msg);


/* Prototoye of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t ovenIsInSuper(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar);


#endif
