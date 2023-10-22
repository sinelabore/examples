/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p EA -A -t Model:Class Model:first_example_step3 -o oven first_example_step3.xml   */
/* This file is generated from first_example_step3.xml - do not edit manually  */
/* Generated on: Sun Oct 22 18:00:41 CEST 2023 / Version 6.1.3706 */



/* User defined header text  */
#include <stdint.h>

uint8_t err=0;
/*Enumeration of all node names */
typedef enum {
	HARDWARETEST_ACTIVITYFINAL,
	HARDWARETEST_ACTIVITYINITIAL,
	HARDWARETEST_17E885A3_000D_4746_957A_333B48DA6DDC,
	HARDWARETEST,__END__,
} BRANCHES;


uint8_t hardwareTest(void){

	BRANCHES id;

	id = HARDWARETEST_17E885A3_000D_4746_957A_333B48DA6DDC;
	while(id != __END__) { 
		switch(id) {
			case HARDWARETEST_ACTIVITYFINAL:
				id= __END__;
				break;

			/* Merged from nodes:   TestRadiator TestLamp TestMotor */
			case HARDWARETEST_17E885A3_000D_4746_957A_333B48DA6DDC:
				// test radiator
				// test lamp
				// test motor
				id=HARDWARETEST_ACTIVITYFINAL;
				break;

			default:
				break;

		}
	}
	return err;
}


