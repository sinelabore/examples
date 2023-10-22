/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -c -p CADIFRA -o test first_example_step3.cdd   */
/* This file is generated from first_example_step3.cdd - do not edit manually  */
/* Generated on: Mon Jan 06 19:13:28 CET 2014 */




#ifndef __TEST_EXT_H__ 
#define __TEST_EXT_H__ 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evDoorOpen=0U,
	evDec,
	evTimeout,
	evPwr,
	evDoorClosed,
	evInc,
	TEST_NO_MSG
} TEST_EVENT_T;


#endif
