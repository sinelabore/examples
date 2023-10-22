/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -A -l cx -v -p ASTAH -o oven -t final:oven:selftest oven_model.asta   */
/* This file is generated from oven_model.asta - do not edit manually  */
/* Generated on: Fri Aug 25 08:27:46 CEST 2023 / Version 6.0.5.3690 */



/* User defined header text  */
#include <stdio.h>
#include <stdint.h>

uint8_t hwStatus=0;


/*Enumeration of all node names */
typedef enum {
	SELFTEST_INITIALNODE0,
	SELFTEST_ACTIVITYFINAL0,
	SELFTEST_06CE11CA_3576_40A8_8C17_11346074AE38,
	SELFTEST,__END__,
} BRANCHES;


uint8_t selftest(void){

	BRANCHES id;

	id = SELFTEST_06CE11CA_3576_40A8_8C17_11346074AE38;
	while(id != __END__) { 
		switch(id) {
			case SELFTEST_ACTIVITYFINAL0:
				id= __END__;
				break;

			/* Merged from nodes:   TestMicrowave TestLamp TestSomethingElse */
			case SELFTEST_06CE11CA_3576_40A8_8C17_11346074AE38:
				// test microwave code goes here
				// test lamp code goes here
				// test code goes here
				id=SELFTEST_ACTIVITYFINAL0;
				break;

			default:
				break;

		}
	}
	return hwStatus;
}


