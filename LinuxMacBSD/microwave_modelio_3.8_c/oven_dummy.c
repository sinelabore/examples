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
	DUMMY_INITIAL_NODE,
	DUMMY_ACTION,
	DUMMY_ACTIVITY_FINAL_NODE,
	DUMMY,__END__,
} BRANCHES;


uint8_t dummy(void){

	BRANCHES id;

	id = DUMMY_ACTION;
	while(id != __END__) { 
		switch(id) {
			case DUMMY_ACTION:
				printf("test activity\n");
				id=DUMMY_ACTIVITY_FINAL_NODE;
				break;

			case DUMMY_ACTIVITY_FINAL_NODE:
				id= __END__;
				break;

			default:
				break;

		}
	}
	return hwSta;
}


