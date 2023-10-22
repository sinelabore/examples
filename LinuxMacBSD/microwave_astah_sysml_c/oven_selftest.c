/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -A -l cx -v -p ASTAH -o oven -t final:oven:selftest oven_model.asml   */
/* This file is generated from oven_model.asml - do not edit manually  */
/* Generated on: Sun Oct 22 17:23:36 CEST 2023 / Version 6.1.3706 */



/* User defined header text  */

#include <stdio.h>
#include <stdint.h>

uint8_t hwStatus=0;
/*Enumeration of all node names */
typedef enum {
	SELFTEST_INITIALNODE2,
	SELFTEST_ACTIVITYFINAL0,
	SELFTEST_58BF999A_6397_415A_9B8D_AE3443E7AF9E,
	SELFTEST,__END__,
} BRANCHES;


uint8_t selftest(void){

	BRANCHES id;

	id = SELFTEST_58BF999A_6397_415A_9B8D_AE3443E7AF9E;
	while(id != __END__) { 
		switch(id) {
			case SELFTEST_ACTIVITYFINAL0:
				id= __END__;
				break;

			/* Merged from nodes:   TestMicrowave TestLamp TestSometingElse */
			case SELFTEST_58BF999A_6397_415A_9B8D_AE3443E7AF9E:
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


