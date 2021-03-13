/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p ssc -o test test.xml   */
/* This file is generated from test.xml - do not edit manually*/
/* Generated on: Sat Mar 13 15:08:00 CET 2021 / Version 5.0.6b1 */


#ifndef __TEST_H__
#define __TEST_H__


/* Header to define simple data types */
#include <stdint.h>

/* State definitions */
typedef enum{
	Active,
	Inactive,
	Off,
	On,
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
	TEST_ENTRY_FLAG_T activeEntry;
	TEST_STATEVAR_T stateVar;
	TEST_STATEVAR_T stateVarActive;
	TEST_INST_ID_T inst_id;
} TEST_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define TEST_INSTANCEDATA_INIT {\
	1,\
	(TEST_STATEVAR_T) Active /* set init state of top state */,\
	(TEST_STATEVAR_T) Off /* set init state of Active */,\
	0 /* instance id */ } 

/* Helper(s) to reset history */
#define TEST_RESET_HISTORY_ACTIVE(instance) ((&instance)->stateVarActive=Off)

/*Prototype of the state machine function */
void  test(TEST_INSTANCEDATA_T *instanceVar, TEST_EVENT_T  msg);


/*Prototoye of the reset machine function */
void testResetMachine(TEST_INSTANCEDATA_T * const instanceVar);

// Helper(s) to reset history
void testResetHistoryActive(TEST_INSTANCEDATA_T * const instanceVar);

// Helper(s) to find out if the machine is in a certain state
uint8_t testIsInActive(const TEST_INSTANCEDATA_T * const instanceVar);
uint8_t testIsInInactive(const TEST_INSTANCEDATA_T * const instanceVar);
uint8_t testIsInOff(const TEST_INSTANCEDATA_T * const instanceVar);
uint8_t testIsInOn(const TEST_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define TEST_IS_IN_ACTIVE(instance)((((&instance)->stateVar==Active)) ? (1U) : (0U))
#define TEST_IS_IN_INACTIVE(instance)((((&instance)->stateVar==Inactive)) ? (1U) : (0U))
#define TEST_IS_IN_OFF(instance)((((&instance)->stateVarActive==Off)&&((&instance)->stateVar==Active)) ? (1U) : (0U))
#define TEST_IS_IN_ON(instance)((((&instance)->stateVarActive==On)&&((&instance)->stateVar==Active)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
TEST_STATES_T testGetInnermostActiveState(const TEST_INSTANCEDATA_T * const instanceVar);

#endif
