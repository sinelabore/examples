/*
 * (c) Peter Mueller, 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -t Model:Class Model:decoder -p EA -o decoder decoder.xml   */
/* This file is generated from decoder.xml - do not edit manually*/
/* Generated on: Wed Dec 28 17:29:00 CET 2022 / Version 5.5.5.6 */


#ifndef DECODER_H
#define DECODER_H


/* Header to define simple data types */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	Day,
	DayOfWeek,
	Hour,
	IgnoreBBKMeteo,
	MinDetected,
	Minute,
	Month,
	WaitForMinStart,
	Year,
	__DECODER_LAST_STATE__} DECODER_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef PROVIDE_OWN_DECODER_STATEMACHINE_TYPES
	typedef uint8_t DECODER_ENTRY_FLAG_T;
	typedef DECODER_STATES_T DECODER_STATEVAR_T;
	typedef uint8_t DECODER_INST_ID_T;
	typedef uint8_t DECODER_EV_CONSUMED_FLAG_T ;
#endif




/* Instance data definition of state machine */
typedef struct{
	DECODER_ENTRY_FLAG_T waitforminstartEntry;
	DECODER_STATEVAR_T stateVar;
	DECODER_STATEVAR_T stateVarMinDetected;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	DECODER_INST_ID_T inst_id;
} DECODER_INSTANCEDATA_T;

/* Helper to initialize the machine's instance data */
#define DECODER_INSTANCEDATA_INIT {\
	1,\
	(DECODER_STATEVAR_T) WaitForMinStart /* set init state of top state */,\
	(DECODER_STATEVAR_T) IgnoreBBKMeteo /* set init state of MinDetected */,\
	0 /* instance id */ } 


/* Helper(s) to reset history */
#define DECODER_RESET_HISTORY_MINDETECTED(instance) ((&(instance))->stateVarMinDetected=IgnoreBBKMeteo)

/*Prototype of the state machine function */
void  decoder(void);


/*Prototoye of the reset machine function */
void decoderResetMachine(void);

/* Helper(s) to reset history */
void decoderResetHistoryMinDetected(void);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t decoderIsInWaitForMinStart(void);
uint8_t decoderIsInYear(void);
uint8_t decoderIsInMinDetected(void);
uint8_t decoderIsInMinute(void);
uint8_t decoderIsInMonth(void);
uint8_t decoderIsInDay(void);
uint8_t decoderIsInIgnoreBBKMeteo(void);
uint8_t decoderIsInDayOfWeek(void);
uint8_t decoderIsInHour(void);


/* Macros to test if the machine is in a certain state. */
#define DECODER_IS_IN_WAITFORMINSTART(instance)((((&instance)->stateVar==WaitForMinStart)) ? (1U) : (0U))
#define DECODER_IS_IN_YEAR(instance)((((&instance)->stateVarMinDetected==Year)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_MINDETECTED(instance)((((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_MINUTE(instance)((((&instance)->stateVarMinDetected==Minute)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_MONTH(instance)((((&instance)->stateVarMinDetected==Month)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_DAY(instance)((((&instance)->stateVarMinDetected==Day)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_IGNOREBBKMETEO(instance)((((&instance)->stateVarMinDetected==IgnoreBBKMeteo)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_DAYOFWEEK(instance)((((&instance)->stateVarMinDetected==DayOfWeek)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))
#define DECODER_IS_IN_HOUR(instance)((((&instance)->stateVarMinDetected==Hour)&&((&instance)->stateVar==MinDetected)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
DECODER_STATES_T decoderGetInnermostActiveState(void);

#endif
