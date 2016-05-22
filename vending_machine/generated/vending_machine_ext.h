/* (C) Your company*//* Command line options: -l cx -v -p ASTAH -o vending_machine -t machine:vending_machine:ui ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sun May 22 11:37:53 CEST 2016 / version 3.6.14 */




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
