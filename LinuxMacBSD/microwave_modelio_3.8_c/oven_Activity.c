/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -A -l cx -v -p Modelio -o oven -t OvenClass oven.xmi   */
/* This file is generated from oven.xmi - do not edit manually  */
/* Generated on: Mon Sep 23 20:03:21 CEST 2019 / version 4.0 */



/* Include default headers (no header definition in UML activity provided) */
/* Define a header comment in your activity model to define own includes here. */

#include <stdint.h>
/*Enumeration of all node names */
typedef enum {
	ACTIVITY_STEP1,
	ACTIVITY_INITIAL_NODE,
	ACTIVITY_DECISION_MERGE,
	ACTIVITY_STEP2,
	ACTIVITY_STEP3,
	ACTIVITY_FLOW_FINAL_NODE,
	ACTIVITY_LOOP_NODE,
	ACTIVITY_CN,
	ACTIVITY,__END__,
} BRANCHES;


uint8_t Activity(void){

	BRANCHES id;

	id = ACTIVITY_STEP1;
	while(id != __END__) { 
		switch(id) {
			case ACTIVITY_STEP1:
				// step 1 action
				printf("step1\n");
				if(1==0){
					id = ACTIVITY_CN;
				}else if(1==1){
					id = ACTIVITY_STEP2;
				}else{
					id = ACTIVITY_STEP3;
				}
				break;

			case ACTIVITY_STEP2:
				printf("Step 2\n");
				id=ACTIVITY_LOOP_NODE;
				break;

			case ACTIVITY_STEP3:
				printf("Step 3\n");
				id=ACTIVITY_FLOW_FINAL_NODE;
				break;

			case ACTIVITY_FLOW_FINAL_NODE:
				id= __END__;
				break;

			case ACTIVITY_LOOP_NODE:
				/* Loop setup code  */
				uint8_t n=0;

				while(n>10){
					/* Loop body code  */
					printf("Inside Loop\n");
				}

				id=ACTIVITY_FLOW_FINAL_NODE;
				break;

			case ACTIVITY_CN:
				if(i==3){
					printf("CN Action 1\n");
				}else if(i==4){
					printf("CN Action 2\n");
				}
				id=ACTIVITY_FLOW_FINAL_NODE;
				break;

			default:
				break;

		}
	}
	return hwStatus;
}


