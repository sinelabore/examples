/* (c) Peter Mueller, 2012 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -o oven.xml first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually  */
/* Generated on: Thu Mar 07 18:11:53 CET 2013 */




#ifndef __OVEN.XML_EXT_H__ 
#define __OVEN.XML_EXT_H__ 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evDoorOpen=0U,
	evDec,
	evTimeout,
	evPwr,
	evDoorClosed,
	evInc,
	OVEN.XML_NO_MSG
} OVEN.XML_EVENT_T;


#endif
