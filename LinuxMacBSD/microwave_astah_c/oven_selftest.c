/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2024
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -A -l cx -v -p ASTAH -o oven -t final:oven:selftest oven_model.asta   */
/* This file is generated from oven_model.asta - do not edit manually  */
/* Generated on: Mon Apr 01 17:59:19 CEST 2024 / Version 6.3.2.3814 */



/* User defined header text  */
#include <stdio.h>
#include <stdint.h>

uint8_t hwStatus=0;


/*Enumeration of all node names */
typedef enum {
	SELFTEST_INITIALNODE0,
	SELFTEST_ACTIVITYFINAL0,
	SELFTEST_42D96F03_F47D_4974_847D_CBAB5F2B8AF4,
	SELFTEST,__END__,
} BRANCHES;


uint8_t selftest(void){

	BRANCHES id;

	id = SELFTEST_42D96F03_F47D_4974_847D_CBAB5F2B8AF4;
	while(id != __END__) { 
		switch(id) {
			case SELFTEST_ACTIVITYFINAL0:
				id= __END__;
				break;

			/* Merged from nodes:   TestMicrowave TestLamp TestSomethingElse */
			case SELFTEST_42D96F03_F47D_4974_847D_CBAB5F2B8AF4:
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


