/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -Trace -p CADIFRA -o complex complex.cdd   */
/* This file is generated from complex.cdd - do not edit manually*/
/* Generated on: Wed May 14 21:43:07 CEST 2025 / 6.5.3 */


#ifndef COMPLEX_H
#define COMPLEX_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>
#include <stdbool.h>

/* State definitions */
typedef enum{
	S1,
	S11,
	S12,
	S2,
	S21,
	S22,
	S3,
	S4,
	__COMPLEX_LAST_STATE__} COMPLEX_STATES_T;






/* Predefined types required by the codegen. You can provide your own  */
/* definitions by setting the following define somewhere in your built env. */
#ifndef PROVIDE_OWN_COMPLEX_STATEMACHINE_TYPES
	typedef uint8_t COMPLEX_ENTRY_FLAG_T;
	typedef COMPLEX_STATES_T COMPLEX_STATEVAR_T;
	typedef uint8_t COMPLEX_INST_ID_T;
	typedef uint8_t COMPLEX_EV_CONSUMED_FLAG_T ;
#endif

/* Instance data definition of state machine */
typedef struct {
	COMPLEX_ENTRY_FLAG_T s1Entry;
	COMPLEX_STATEVAR_T stateVar;
	COMPLEX_STATEVAR_T stateVarS1;
	COMPLEX_STATEVAR_T stateVarS2;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	COMPLEX_INST_ID_T inst_id;
} COMPLEX_INSTANCEDATA_T ;

/* Helper to initialize the machine's instance data */
#define COMPLEX_INSTANCEDATA_INIT {\
	1,\
	(COMPLEX_STATEVAR_T) S1 /* set init state of top state */,\
	(COMPLEX_STATEVAR_T) S12 /* set init state of S1 */,\
	(COMPLEX_STATEVAR_T) S21 /* set init state of S2 */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void complexInitMachine(COMPLEX_INSTANCEDATA_T * const instanceVar, COMPLEX_INST_ID_T instId);

/* Helper(s) to reset history */
#define COMPLEX_RESET_HISTORY_S1(instance) ((&(instance))->stateVarS1=S12)
#define COMPLEX_RESET_HISTORY_S2(instance) ((&(instance))->stateVarS2=S21)

/* Prototype of the state machine function */
void  complex(COMPLEX_INSTANCEDATA_T *instanceVar, COMPLEX_EVENT_T  msg);


/* Prototoye of the reset machine function */
void complexResetMachine(COMPLEX_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to reset history */
void complexResetHistoryS1(COMPLEX_INSTANCEDATA_T * const instanceVar);
void complexResetHistoryS2(COMPLEX_INSTANCEDATA_T * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t complexIsInS2(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS21(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS22(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS1(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS11(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS12(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS3(const COMPLEX_INSTANCEDATA_T * const instanceVar);
uint8_t complexIsInS4(const COMPLEX_INSTANCEDATA_T * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define COMPLEX_IS_IN_S2(instance)((((&instance)->stateVar==S2)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S21(instance)((((&instance)->stateVarS2==S21)&&((&instance)->stateVar==S2)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S22(instance)((((&instance)->stateVarS2==S22)&&((&instance)->stateVar==S2)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S1(instance)((((&instance)->stateVar==S1)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S11(instance)((((&instance)->stateVarS1==S11)&&((&instance)->stateVar==S1)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S12(instance)((((&instance)->stateVarS1==S12)&&((&instance)->stateVar==S1)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S3(instance)((((&instance)->stateVar==S3)) ? (1U) : (0U))
#define COMPLEX_IS_IN_S4(instance)((((&instance)->stateVar==S4)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
COMPLEX_STATES_T complexGetInnermostActiveState(const COMPLEX_INSTANCEDATA_T * const instanceVar);


#endif
