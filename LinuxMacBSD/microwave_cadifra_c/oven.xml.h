/* (c) Peter Mueller, 2012 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven.xml first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually*/
/* Generated on: Thu Mar 07 18:11:53 CET 2013 / version 3.11 */


#ifndef __OVEN.XML_H__
#define __OVEN.XML_H__


/* Header to define simple data types */
#include <stdint.h>

/* State definitions */
typedef enum{
	Super,
	Completed,
	CookingPause,
	Idle,
	Cooking,
	__OVEN.XML_LAST_STATE__} OVEN.XML_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef __PROVIDE_OWN_OVEN.XML_STATEMACHINE_TYPES__
	typedef uint8_t OVEN.XML_ENTRY_FLAG_T;
	typedef OVEN.XML_STATES_T OVEN.XML_STATEVAR_T;
	typedef uint8_t OVEN.XML_INST_ID_T;
	typedef uint8_t OVEN.XML_EV_CONSUMED_FLAG_T ;
#endif




/* Instance data definition of state machine */
typedef struct{
	OVEN.XML_ENTRY_FLAG_T superEntry;
	OVEN.XML_STATEVAR_T stateVar;
	OVEN.XML_STATEVAR_T stateVarSuper;
	OVEN.XML_INST_ID_T inst_id;
} OVEN.XML_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define OVEN.XML_INSTANCEDATA_INIT {\
	1,\
	(OVEN.XML_STATEVAR_T) Super /* set init state of top state */,\
	(OVEN.XML_STATEVAR_T) Idle /* set init state of Super */,\
	0 /* instance id */ } 

/* Helper(s) to reset history */
#define OVEN.XML_RESET_HISTORY_SUPER(instance) ((&instance)->stateVarSuper=Idle)
/* Region handler prototypes  */

/*Prototype of the state machine function */
void  oven.xml(OVEN.XML_INSTANCEDATA_T *instanceVar);


/*Prototoye of the reset machine function */
void oven.xmlResetMachine(OVEN.XML_INSTANCEDATA_T *instanceVar);

// Helper(s) to reset history
void oven.xmlResetHistorySuper(OVEN.XML_INSTANCEDATA_T *instanceVar);

// Helper(s) to find out if the machine is in a certain state
uint8_t oven.xmlIsInSuper(OVEN.XML_INSTANCEDATA_T *instanceVar);
uint8_t oven.xmlIsInCompleted(OVEN.XML_INSTANCEDATA_T *instanceVar);
uint8_t oven.xmlIsInCookingPause(OVEN.XML_INSTANCEDATA_T *instanceVar);
uint8_t oven.xmlIsInIdle(OVEN.XML_INSTANCEDATA_T *instanceVar);
uint8_t oven.xmlIsInCooking(OVEN.XML_INSTANCEDATA_T *instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN.XML_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN.XML_IS_IN_COMPLETED(instance)((((&instance)->stateVarSuper==Completed)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN.XML_IS_IN_COOKINGPAUSE(instance)((((&instance)->stateVarSuper==CookingPause)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN.XML_IS_IN_IDLE(instance)((((&instance)->stateVarSuper==Idle)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN.XML_IS_IN_COOKING(instance)((((&instance)->stateVarSuper==Cooking)&&((&instance)->stateVar==Super)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN.XML_STATES_T oven.xmlGetInnermostActiveState(OVEN.XML_INSTANCEDATA_T *instanceVar);

#endif
