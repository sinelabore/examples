/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -p UModel -A -l cx -t MicrowaveOven:MachineClass -o oven oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually  */
/* Generated on: Fri Aug 25 08:41:13 CEST 2023 / Version 6.0.5.3690 */



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


