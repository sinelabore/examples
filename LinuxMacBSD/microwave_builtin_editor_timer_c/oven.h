/* (C) Your company *//* Command line options: -p ssc -l cx -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually*/
/* Generated on: Sat Nov 14 15:02:15 CET 2026 / 8.0 */


#ifndef OVEN_H
#define OVEN_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>

/* Timer service, see UseTimerService */
#include "timer_service.h"

/* Timer ids: the named timers, then one per timed transition without a name */
#define OVEN_TM_COOKTIMER 0U /* cookTimer: Cooking: after() -> Completed */
#define OVEN_TM_COOKING_0 1U /* Cooking: every(1 s) internal */
#define OVEN_TM_COUNT 2U
#define OVEN_TM_NAMED_COUNT 1U

/* Timer names as the model writes them, for calls of the timer service in the
   application's own code. Keep them unique in the project. */
#define cookTimer OVEN_TM_COOKTIMER

/* The model's literal durations must fit TS_TICKS_T: one that does not fails to
   compile here; see TS_TICKS_TYPE and TS_TICK_US in timer_service.h. */
typedef char OVEN_TS_FITS_0[(TS_S_FITS(1U)) ? 1 : -1]; /* TS_S(1U) */

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

	/* Timers (UseTimerService) */
	uint8_t timer[OVEN_TM_COUNT]; /* TS_IDLE, TS_RUNNING, TS_EXPIRED */
	TS_TICKS_T timerTime[OVEN_TM_NAMED_COUNT]; /* a named timer's time while it does not run */
	TS_SERVICE_T * timerService; /* set before the first call of the machine */

	OVEN_INST_ID_T inst_id;
} OVEN_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Super /* set init state of top state */,\
	(OVEN_STATEVAR_T) Idle /* set init state of Super */,\
	{0U} /* timers */,\
	{0U} /* timer times */,\
	NULL /* timerService */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId);

/* Hand a timeout from the timer service to the machine; ignored unless the timer runs. */
void ovenSetTimeout(OVEN_INSTANCEDATA_T * const instanceVar, uint16_t id);

/* 1 while a timeout waits for a call of the machine without event (NO_MSG). */
uint8_t ovenTimeoutPending(const OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
#define OVEN_RESET_HISTORY_SUPER(instance) ((&(instance))->stateVarSuper=Idle)

/* Prototype of the state machine function */
OVEN_EV_CONSUMED_FLAG_T oven(OVEN_INSTANCEDATA_T *instanceVar, const OVEN_MSG_T *userData);


/* Prototoye of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
void ovenResetHistorySuper(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t ovenIsInCompleted(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInIdle(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInSuper(const OVEN_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_COMPLETED(instance)((((&instance)->stateVarSuper==Completed)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKING(instance)((((&instance)->stateVarSuper==Cooking)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_COOKINGPAUSE(instance)((((&instance)->stateVarSuper==CookingPause)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_IDLE(instance)((((&instance)->stateVarSuper==Idle)&&((&instance)->stateVar==Super)) ? (1U) : (0U))
#define OVEN_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar);


#endif
