/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -A -l cx -v -p ASTAH -o activity -t machine:coin_store:change_algorithm vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sat May 21 23:01:49 CEST 2016 / version 3.6.14 */



/* User defined header text  */
#include <stdio.h>
#include <stdlib.h>

#include "activity_change_algorithm.h"

uint8_t coins[] =   {50, 20, 10};
uint8_t available[] =   {5, 60, 4}; // available number of coins
uint8_t output[] =     {0, 0, 0};

uint8_t i;
/*Enumeration of all node names */
typedef enum {
	CHANGE_ALGORITHM_INITIALNODE0,
	CHANGE_ALGORITHM_C1,
	CHANGE_ALGORITHM_SET_OUTPUT_IF_NO_CHANGE_POSSIBLE,
	CHANGE_ALGORITHM_SET_OUTPUT_IF_CHANGE_POSSIBLE,
	CHANGE_ALGORITHM_UPDATE_CHANGE,
	CHANGE_ALGORITHM_C3,
	CHANGE_ALGORITHM_PRINTF_ERROR,
	CHANGE_ALGORITHM_RETURN_CHANGE,
	CHANGE_ALGORITHM_ACTIVITYFINAL2,
	CHANGE_ALGORITHM_INIT_LOOP_CNT,
	CHANGE_ALGORITHM_C2,
	CHANGE_ALGORITHM_INCREMENT_LOOP_CNT,
	CHANGE_ALGORITHM,__END__,
} BRANCHES;


void change_algorithm(uint8_t change){

	BRANCHES id;

	id = CHANGE_ALGORITHM_INIT_LOOP_CNT;
	while(id != __END__) { 
		switch(id) {
			case CHANGE_ALGORITHM_C1:
				if(change/coins[i] > available[i]){
					id = CHANGE_ALGORITHM_SET_OUTPUT_IF_NO_CHANGE_POSSIBLE;
				}else{
					id = CHANGE_ALGORITHM_SET_OUTPUT_IF_CHANGE_POSSIBLE;
				}
				break;

			case CHANGE_ALGORITHM_SET_OUTPUT_IF_NO_CHANGE_POSSIBLE:
				output[i]=0;
				id=CHANGE_ALGORITHM_UPDATE_CHANGE;
				break;

			case CHANGE_ALGORITHM_SET_OUTPUT_IF_CHANGE_POSSIBLE:
				output[i] = change/coins[i];
				id=CHANGE_ALGORITHM_UPDATE_CHANGE;
				break;

			case CHANGE_ALGORITHM_UPDATE_CHANGE:
				change -= output[i]*coins[i];
				id=CHANGE_ALGORITHM_C2;
				break;

			case CHANGE_ALGORITHM_C3:
				if(change!=0){
					id = CHANGE_ALGORITHM_PRINTF_ERROR;
				}else{
					id = CHANGE_ALGORITHM_RETURN_CHANGE;
				}
				break;

			case CHANGE_ALGORITHM_PRINTF_ERROR:
				puts ("Fehler: Kann nicht passend herausgeben");
				id=CHANGE_ALGORITHM_ACTIVITYFINAL2;
				break;

			case CHANGE_ALGORITHM_RETURN_CHANGE:
				printf("Change Algorithm::  Return Change\n");
				for (i = 0; i<3; i++)
					printf ("Change Algorithm::  %d times %d Cent(s)\n", output[i], coins[i]);
				id=CHANGE_ALGORITHM_ACTIVITYFINAL2;
				break;

			case CHANGE_ALGORITHM_ACTIVITYFINAL2:
				id= __END__;
				break;

			case CHANGE_ALGORITHM_INIT_LOOP_CNT:
				i=0;
				id=CHANGE_ALGORITHM_C1;
				break;

			case CHANGE_ALGORITHM_C2:
				if(i<3){
					id = CHANGE_ALGORITHM_INCREMENT_LOOP_CNT;
				}else{
					id = CHANGE_ALGORITHM_C3;
				}
				break;

			case CHANGE_ALGORITHM_INCREMENT_LOOP_CNT:
				i++;
				id=CHANGE_ALGORITHM_C1;
				break;

			default:
				break;

		}
	}
	return ;
}


