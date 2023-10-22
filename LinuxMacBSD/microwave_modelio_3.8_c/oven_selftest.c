/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -A -l cx -v -p Modelio -o oven -t OvenClass oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually  */
/* Generated on: Sun Oct 22 18:14:38 CEST 2023 / Version 6.1.3706 */



/* User defined header text  */
#include <stdio.h>
#include <stdint.h>

uint8_t hwSta=0;
uint8_t n=0;
uint8_t i=0;

/*Enumeration of all node names */
typedef enum {
	SELFTEST_STEP1,
	SELFTEST_INITIAL_NODE,
	SELFTEST_DECISION_MERGE,
	SELFTEST_STEP2,
	SELFTEST_STEP3,
	SELFTEST_FLOW_FINAL_NODE,
	SELFTEST_LOOP_NODE,
	SELFTEST_CN,
	SELFTEST,__END__,
} BRANCHES;


uint8_t selftest(void){

	BRANCHES id;

	id = SELFTEST_STEP1;
	while(id != __END__) { 
		switch(id) {
			case SELFTEST_STEP1:
				// step 1 action
				printf("step1 self-test\n");
				if(1==0){
					id = SELFTEST_CN;
				}else if(1==1){
					id = SELFTEST_STEP2;
				}else{
					id = SELFTEST_STEP3;
				}
				break;

			case SELFTEST_STEP2:
				printf("Step 2a selftest\n");
				id=SELFTEST_LOOP_NODE;
				break;

			case SELFTEST_STEP3:
				printf("Step 2b selftest\n");
				id=SELFTEST_FLOW_FINAL_NODE;
				break;

			case SELFTEST_FLOW_FINAL_NODE:
				id= __END__;
				break;

			case SELFTEST_LOOP_NODE:
				/* Loop setup code  */
				n=0;

				while(n<10){
					/* Loop body code  */
					n++;
					printf("Inside Loop\n");
				}

				id=SELFTEST_FLOW_FINAL_NODE;
				break;

			case SELFTEST_CN:
				if(i==3){
					printf("CN Action 1\n");
				}else if(i==4){
					printf("CN Action 2\n");
				}
				id=SELFTEST_FLOW_FINAL_NODE;
				break;

			default:
				break;

		}
	}
	return hwSta;
}


