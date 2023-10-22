/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -c -p CADIFRA -o test first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually*/
/* Generated on: Mon Jan 06 19:13:28 CET 2014 / version 3.5 */


#ifndef __TEST_H__
#define __TEST_H__


/* Header to define simple data types */
#include <stdint.h>

/* State definitions */
typedef enum{
	Super,
	Completed,
	CookingPause,
	Idle,
	Error,
	Cooking,
	__TEST_LAST_STATE__} TEST_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef __PROVIDE_OWN_TEST_STATEMACHINE_TYPES__
	typedef uint8_t TEST_ENTRY_FLAG_T;
	typedef TEST_STATES_T TEST_STATEVAR_T;
	typedef uint8_t TEST_INST_ID_T;
	typedef uint8_t TEST_EV_CONSUMED_FLAG_T ;
#endif




/* Instance data definition of state machine */
typedef struct{
	TEST_ENTRY_FLAG_T superEntry;
	TEST_STATEVAR_T stateVar;
	TEST_STATEVAR_T stateVarSuper;
	TEST_INST_ID_T inst_id;
} TEST_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define TEST_INSTANCEDATA_INIT {\
	1,\
	(TEST_STATEVAR_T) Super /* set init state of top state */,\
	(TEST_STATEVAR_T) Idle /* set init state of Super */,\
	0 /* instance id */ } 

/* Helper(s) to reset history */
#define TEST_RESET_HISTORY_SUPER(instance) ((&instance)->stateVarSuper=Idle)
/* Region handler prototypes  */

/*Prototype of the state machine function */
void  test(TEST_INSTANCEDATA_T *instanceVar);


/*Prototoye of the reset machine function */
void testResetMachine(TEST_INSTANCEDATA_T *instanceVar);

// Helper(s) to reset history
void testResetHistorySuper(TEST_INSTANCEDATA_T *instanceVar);

// Helper(s) to find out if the machine is in a certain state
uint8_t testIsInSuper(TEST_INSTANCEDATA_T *instanceVar);
uint8_t testIsInCompleted(TEST_INSTANCEDATA_T *instanceVar);
uint8_t testIsInCookingPause(TEST_INSTANCEDATA_T *instanceVar);
uint8_t testIsInIdle(TEST_INSTANCEDATA_T *instanceVar);
uint8_t testIsInError(TEST_INSTANCEDATA_T *instanceVar);
uint8_t testIsInCooking(TEST_INSTANCEDATA_T *instanceVar);


/* Macros to test if the machine is in a certain state. */
#define TEST_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))
#define TEST_IS_IN_COMPLETED(instance)((((&instance)->stateVarSuper==Completed)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define TEST_IS_IN_COOKINGPAUSE(instance)((((&instance)->stateVarSuper==CookingPause)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define TEST_IS_IN_IDLE(instance)((((&instance)->stateVarSuper==Idle)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define TEST_IS_IN_ERROR(instance)((((&instance)->stateVar==Error)) ? (1U) : (0U))
#define TEST_IS_IN_COOKING(instance)((((&instance)->stateVarSuper==Cooking)&&((&instance)->stateVar==Super)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
TEST_STATES_T testGetInnermostActiveState(TEST_INSTANCEDATA_T *instanceVar);

#endif
