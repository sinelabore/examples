/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -E -p ssc   */
/* This file is generated from state_machine.xml - do not edit manually  */
/* Generated on: Tue Mar 07 15:02:52 CET 2023 / Version 5.5.6.3515 */




#ifndef STATE_MACHINE_EXT_H 
#define STATE_MACHINE_EXT_H 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evShutdown=0U,
	evTimeout,
	evFast,
	evStart,
	evStop,
	evSlow,
	STATE_MACHINE_NO_MSG
} STATE_MACHINE_EVENT_T;


#endif
