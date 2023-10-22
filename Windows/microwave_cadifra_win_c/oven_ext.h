/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p CADIFRA -L ../../bin/License.txt -p Cadifra -l cx -doxygen -o oven first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually  */
/* Generated on: Fri Aug 25 09:05:30 CEST 2023 / Version 6.0.5.3690 */




#ifndef OVEN_EXT_H 
#define OVEN_EXT_H 

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


#endif
