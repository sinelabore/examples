/*
 * (c) Peter Mueller, 2008
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -p UModel -A -l cx -t MicrowaveOven:MachineClass -doxygen -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually  */
/* Generated on: Sun Dec 15 20:35:28 CET 2024 / Version 6.4.1B1 */



/* User defined header text  */

#include <stdint.h>
#include <stdio.h>

uint8_t testResult=0;

/*Enumeration of all node names */
typedef enum {
	SELFTEST_INITIAL_0,
	SELFTEST_SELFTESTSTEP1,
	SELFTEST_FINAL_0,
	SELFTEST,__END__,
} BRANCHES;


uint8_t selftest(void){

	BRANCHES id;

	id = SELFTEST_SELFTESTSTEP1;
	while(id != __END__) { 
		switch(id) {
			case SELFTEST_SELFTESTSTEP1:
				printf("Selftest was ok\n");
				id=SELFTEST_FINAL_0;
				break;

			case SELFTEST_FINAL_0:
				id= __END__;
				break;

			default:
				break;

		}
	}
	return 0 ;
}


