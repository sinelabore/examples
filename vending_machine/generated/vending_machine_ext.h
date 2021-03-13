/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -l cx -v -p ASTAH -o vending_machine -t machine:vending_machine:ui ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sat Mar 13 15:41:47 CET 2021 / Version 5.0.6b1 */




#ifndef __VENDING_MACHINE_EXT_H__ 
#define __VENDING_MACHINE_EXT_H__ 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evReleaseDone=0U,
	evProductA,
	evCoin,
	evUnknownCoin,
	evProductC,
	evClearSelection,
	evProductB,
	VENDING_MACHINE_NO_MSG
} VENDING_MACHINE_EVENT_T;


#endif
