/* (c) Peter Mueller, 2010 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven oven.cdd   */
/* This file is generated from oven.h - do not edit manually*/
/* Generated on: Wed Nov 24 21:11:29 CET 2010 / version 2.2 */


#ifndef __OVEN_H__
#define __OVEN_H__
/* State definitions */
typedef enum{
	Super,
	Completed,
	CookingPause,
	Idle,
	Cooking
} OVEN_STATES_T;




/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef __PROVIDE_OWN_OVEN_STATEMACHINE_TYPES__
	typedef unsigned char OVEN_ENTRY_FLAG_T;
	typedef OVEN_STATES_T OVEN_STATEVAR_T;
	typedef unsigned char OVEN_INST_ID_T;
	typedef unsigned char OVEN_EV_CONSUMED_FLAG_T;
#endif




/* Instance data definition of state machine */
typedef struct{
	OVEN_ENTRY_FLAG_T superEntry;
	OVEN_STATEVAR_T stateVar;
	OVEN_STATEVAR_T stateVarSuper;
	OVEN_INST_ID_T inst_id;
} OVEN_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Super /* set init state of top state */,\
	(OVEN_STATEVAR_T) Idle /* set init state of Super */,\
	0 /* instance id */ } 

/* Helper(s) to reset history */
#define OVEN_RESET_HISTORY_SUPER(instance) ((&instance)->stateVarSuper=Idle)

/*Prototypes for the change state function*/
void  ovenChangeToState(OVEN_INSTANCEDATA_T *instanceVar, OVEN_STATES_T state);
void  ovenChangeToStateSuper(OVEN_INSTANCEDATA_T *instanceVar, OVEN_STATES_T state);

/*Prototype of the state machine function */
void   oven(OVEN_INSTANCEDATA_T *instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COMPLETED(instance)((((&instance)->stateVarSuper==Completed)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKINGPAUSE(instance)((((&instance)->stateVarSuper==CookingPause)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_IDLE(instance)((((&instance)->stateVarSuper==Idle)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKING(instance)((((&instance)->stateVarSuper==Cooking)&&((&instance)->stateVar==Super)) ? (1U) : (0U))

#endif
