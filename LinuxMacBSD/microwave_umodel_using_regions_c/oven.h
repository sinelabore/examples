/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p UModel -l cx -t MicrowaveOvenWithRegions:MachineClass -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually*/
/* Generated on: Sun Oct 22 18:28:05 CEST 2023 / Version 6.1.3706 */


#ifndef OVEN_H
#define OVEN_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	Active,
	Cooking,
	CookingPause,
	HighPower,
	Inactive,
	LightOff,
	LightOn,
	LowPower,
	RadiatorOff,
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
	OVEN_ENTRY_FLAG_T inactiveEntry;
	OVEN_STATEVAR_T stateVar;
	OVEN_STATEVAR_T stateVarLight;
	OVEN_STATEVAR_T stateVarPower;
	OVEN_STATEVAR_T stateVarRadiator;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	OVEN_INST_ID_T inst_id;
} OVEN_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Inactive /* set init state of top state */,\
	(OVEN_STATEVAR_T) LightOn /* set init state of Light */,\
	(OVEN_STATEVAR_T) LowPower /* set init state of Power */,\
	(OVEN_STATEVAR_T) RadiatorOff /* set init state of Radiator */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void ovenInitMachine(OVEN_INSTANCEDATA_T * const instanceVar, OVEN_INST_ID_T instId);


/* Prototype of the state machine function */
void  oven(OVEN_INSTANCEDATA_T *instanceVar);


/* Prototoye of the reset machine function */
void ovenResetMachine(OVEN_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t ovenIsInActive(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInLightOn(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInLightOff(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInHighPower(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInLowPower(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInRadiatorOff(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCookingPause(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInCooking(const OVEN_INSTANCEDATA_T * const instanceVar);
uint8_t ovenIsInInactive(const OVEN_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_ACTIVE(instance)((((&instance)->stateVar==Active)) ? (1U) : (0U))
#define OVEN_IS_IN_INACTIVE(instance)((((&instance)->stateVar==Inactive)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN_STATES_T ovenGetInnermostActiveState(const OVEN_INSTANCEDATA_T * const instanceVar);


#endif
