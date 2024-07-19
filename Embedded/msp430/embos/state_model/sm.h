/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p ssc sm.xml   */
/* This file is generated from sm.xml - do not edit manually*/
/* Generated on: Fri Jul 19 18:23:30 CEST 2024 / Version 6.3.3. */


#ifndef SM_H
#define SM_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	FAST,
	ON,
	SLOW,
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
	SM_ENTRY_FLAG_T onEntry;
	SM_STATEVAR_T stateVar;
	
	/* Start of user defined attributes */
	/* End of user defined attributes */

	SM_INST_ID_T inst_id;
} SM_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define SM_INSTANCEDATA_INIT {\
	1,\
	(SM_STATEVAR_T) ON /* set init state of top state */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void smInitMachine(SM_INSTANCEDATA_T * const instanceVar, SM_INST_ID_T instId);


/* Prototype of the state machine function */
void  sm(SM_INSTANCEDATA_T *instanceVar, SM_EVENT_T  msg);


/* Prototoye of the reset machine function */
void smResetMachine(SM_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t smIsInFAST(const SM_INSTANCEDATA_T * const instanceVar);
uint8_t smIsInON(const SM_INSTANCEDATA_T * const instanceVar);
uint8_t smIsInSLOW(const SM_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define SM_IS_IN_FAST(instance)((((&instance)->stateVar==FAST)) ? (1U) : (0U))
#define SM_IS_IN_ON(instance)((((&instance)->stateVar==ON)) ? (1U) : (0U))
#define SM_IS_IN_SLOW(instance)((((&instance)->stateVar==SLOW)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
SM_STATES_T smGetInnermostActiveState(const SM_INSTANCEDATA_T * const instanceVar);


#endif
