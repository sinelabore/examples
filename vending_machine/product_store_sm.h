/* (C) Your company*//* Command line options: -l cx -v -p ASTAH -o product_store_sm -t machine:product_store_sm:sm vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually*/
/* Generated on: Sat May 21 23:01:44 CEST 2016 / version 3.6.14 */


#ifndef __PRODUCT_STORE_SM_H__
#define __PRODUCT_STORE_SM_H__


/* Header to define simple data types */
#include <stdint.h>

/* State definitions */
typedef enum{
	Idle,
	MotorRunning,
	ReleasingOutletDoor,
	__PRODUCT_STORE_SM_LAST_STATE__} PRODUCT_STORE_SM_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef __PROVIDE_OWN_PRODUCT_STORE_SM_STATEMACHINE_TYPES__
	typedef uint8_t PRODUCT_STORE_SM_ENTRY_FLAG_T;
	typedef PRODUCT_STORE_SM_STATES_T PRODUCT_STORE_SM_STATEVAR_T;
	typedef uint8_t PRODUCT_STORE_SM_INST_ID_T;
	typedef uint8_t PRODUCT_STORE_SM_EV_CONSUMED_FLAG_T ;
#endif




/* Instance data definition of state machine */
typedef struct{
	PRODUCT_STORE_SM_ENTRY_FLAG_T idleEntry;
	PRODUCT_STORE_SM_STATEVAR_T stateVar;
		PRODUCT_STORE_SM_INST_ID_T inst_id;
} PRODUCT_STORE_SM_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define PRODUCT_STORE_SM_INSTANCEDATA_INIT {\
	1,\
	(PRODUCT_STORE_SM_STATEVAR_T) Idle /* set init state of top state */,\
	0 /* instance id */ } 

/* Region handler prototypes  */

/*Prototype of the state machine function */
void  product_store_sm(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar, PRODUCT_STORE_SM_EVENT_T msg);


/*Prototoye of the reset machine function */
void product_store_smResetMachine(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar);

// Helper(s) to find out if the machine is in a certain state
uint8_t product_store_smIsInIdle(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar);
uint8_t product_store_smIsInMotorRunning(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar);
uint8_t product_store_smIsInReleasingOutletDoor(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar);


/* Macros to test if the machine is in a certain state. */
#define PRODUCT_STORE_SM_IS_IN_IDLE(instance)((((&instance)->stateVar==Idle)) ? (1U) : (0U))
#define PRODUCT_STORE_SM_IS_IN_MOTORRUNNING(instance)((((&instance)->stateVar==MotorRunning)) ? (1U) : (0U))
#define PRODUCT_STORE_SM_IS_IN_RELEASINGOUTLETDOOR(instance)((((&instance)->stateVar==ReleasingOutletDoor)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
PRODUCT_STORE_SM_STATES_T product_store_smGetInnermostActiveState(PRODUCT_STORE_SM_INSTANCEDATA_T *instanceVar);

#endif
