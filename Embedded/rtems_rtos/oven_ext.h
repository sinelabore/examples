/* (c) Peter Mueller, 2010 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven oven.cdd   */
/* This file is generated from oven.cdd - do not edit manually */
/* Generated on: Wed Nov 24 21:11:29 CET 2010 */


#ifndef __OVEN_EXT_H__ 
#define __OVEN_EXT_H__ 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evDoorOpen=0U,
	evDec,
	evTimeout,
	evPwr,
	evDoorClosed,
	evInc,
	OVEN_NO_MSG
} OVEN_EVENT_T;


#endif
