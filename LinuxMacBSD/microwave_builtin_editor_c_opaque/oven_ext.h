/* Sinelabore Software Tools GmbH *//* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Sun Oct 22 17:49:33 CEST 2023 / Version 6.1.3706 */




#ifndef OVEN_EXT_H 
#define OVEN_EXT_H 


/* Forward declaration of machine instance data struct */
struct OVEN_INSTANCEDATA ;

/*Events which can be sent to the state-machine  */
typedef enum
{
	evTimeout=0U,
	evDec,
	evDoorOpen,
	evDoorClosed,
	evInc,
	evPwr,
	OVEN_NO_MSG
} OVEN_EVENT_T;

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


/* Helper to initialize the machine's instance data */
#define OVEN_INSTANCEDATA_INIT {\
	1,\
	(OVEN_STATEVAR_T) Super /* set init state of top state */,\
	(OVEN_STATEVAR_T) Idle /* set init state of MainRegion */,\
	0 /* instance id */ } 



/* Init function of the state machine. Must be called once before using any other function. */
void ovenInitMachine(struct OVEN_INSTANCEDATA * const instanceVar, OVEN_INST_ID_T instId);


/* Get size of instance variable. */
size_t ovenSizeOf(void);


/* Prototype of the state machine function */
void oven(struct OVEN_INSTANCEDATA *instanceVar, OVEN_EVENT_T  msg);


/* Prototoye of the reset machine function */
void ovenResetMachine(struct OVEN_INSTANCEDATA * const instanceVar);

/* Helper(s) to find out if the machine is in a certain state*/
uint8_t ovenIsInSuper(const struct OVEN_INSTANCEDATA * const instanceVar);
uint8_t ovenIsInCompleted(const struct OVEN_INSTANCEDATA * const instanceVar);
uint8_t ovenIsInCooking(const struct OVEN_INSTANCEDATA * const instanceVar);
uint8_t ovenIsInCookingPause(const struct OVEN_INSTANCEDATA * const instanceVar);
uint8_t ovenIsInIdle(const struct OVEN_INSTANCEDATA * const instanceVar);


/* Macros to test if the machine is in a certain state. */
#define OVEN_IS_IN_SUPER(instance)((((&instance)->stateVar==Super)) ? (1U) : (0U))


/* Helper returning the innermost active state.*/
OVEN_STATES_T ovenGetInnermostActiveState(const struct OVEN_INSTANCEDATA * const instanceVar);

#endif
