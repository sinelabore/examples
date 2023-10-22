/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -A -l cx -v -p ASTAH -o oven -t final:oven:selftest oven_model.asta   */
/* This file is generated from oven_model.asta - do not edit manually  */
/* Generated on: Sun Oct 22 17:21:54 CEST 2023 / Version 6.1.3706 */



/* User defined header text  */
#include <stdio.h>
#include <stdint.h>

uint8_t hwStatus=0;


/*Enumeration of all node names */
typedef enum {
	SELFTEST_INITIALNODE0,
	SELFTEST_ACTIVITYFINAL0,
	SELFTEST_421127AE_5AE5_48D8_B78E_930653DB631C,
	SELFTEST,__END__,
} BRANCHES;


uint8_t selftest(void){

	BRANCHES id;

	id = SELFTEST_421127AE_5AE5_48D8_B78E_930653DB631C;
	while(id != __END__) { 
		switch(id) {
			case SELFTEST_ACTIVITYFINAL0:
				id= __END__;
				break;

			/* Merged from nodes:   TestMicrowave TestLamp TestSomethingElse */
			case SELFTEST_421127AE_5AE5_48D8_B78E_930653DB631C:
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


