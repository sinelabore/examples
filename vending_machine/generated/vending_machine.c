/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -l cx -v -p ASTAH -o vending_machine -t machine:vending_machine:ui ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sat Mar 13 15:41:47 CET 2021 / Version 5.0.6b1 */


#include <stdio.h>
#include <stdlib.h>
#include "vending_machine_ext.h"
#include "vending_machine.h"

#include "display_with_coinbank.h"
#include "product_store.h"

uint8_t price;
uint8_t insertedSum;



void  vending_machine(VENDING_MACHINE_INSTANCEDATA_T *instanceVar, VENDING_MACHINE_EVENT_T  msg){
	
	/*execute entry code of default state once to init machine */
	if(instanceVar->waitforselectionEntry==1U){
		price=0;
		insertedSum=0;
		displayValue(0);
		printf("VendingMachine::  Enter WaitForSelection\n");
		

		instanceVar->waitforselectionEntry=0U;
	}


	switch (instanceVar->stateVar) {

		case WaitForSelection:
			if(msg==(VENDING_MACHINE_EVENT_T)evProductA){

				if(getQuantityProduct(PRODUCT_A)>0){
					/* Transition from WaitForSelection to SelectionDone */
					/* Action code for transition  */
					price=100;
					printf("VendingMachine::   Price=1 Euro\n");
					setSelectedProduct(PRODUCT_A);


					/* OnEntry code of state SelectionDone */
					printf("VendingMachine::  Enter SelectionDone\n");

					/* adjust state variables  */
					instanceVar->stateVar = SelectionDone;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else if(msg==(VENDING_MACHINE_EVENT_T)evProductB){

				if(getQuantityProduct(PRODUCT_B)>0){
					/* Transition from WaitForSelection to SelectionDone */
					/* Action code for transition  */
					price=60;
					printf("VendingMachine::  Price=60 Cent\n");
					setSelectedProduct(PRODUCT_B);


					/* OnEntry code of state SelectionDone */
					printf("VendingMachine::  Enter SelectionDone\n");

					/* adjust state variables  */
					instanceVar->stateVar = SelectionDone;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else if(msg==(VENDING_MACHINE_EVENT_T)evProductC){

				if(getQuantityProduct(PRODUCT_C)>0){
					/* Transition from WaitForSelection to SelectionDone */
					/* Action code for transition  */
					price=80;
					printf("VendingMachine::  Price=80 Cent\n");
					setSelectedProduct(PRODUCT_C);


					/* OnEntry code of state SelectionDone */
					printf("VendingMachine::  Enter SelectionDone\n");

					/* adjust state variables  */
					instanceVar->stateVar = SelectionDone;
				}else{
					/* Intentionally left blank */
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case WaitForSelection  */

		case SelectionDone:
			if(msg==(VENDING_MACHINE_EVENT_T)evClearSelection){
				/* OnExit code of state SelectionDone */
				insertedSum+=getCoinValue();

				/* Transition from SelectionDone to WaitForSelection */
				/* Action code for transition  */
				releaseCoins(insertedSum);


				/* OnEntry code of state WaitForSelection */
				price=0;
				insertedSum=0;
				displayValue(0);
				printf("VendingMachine::  Enter WaitForSelection\n");

				/* adjust state variables  */
				instanceVar->stateVar = WaitForSelection;
			}else if(msg==(VENDING_MACHINE_EVENT_T)evCoin){
				/* OnExit code of state SelectionDone */
				insertedSum+=getCoinValue();

				if(insertedSum==price){
					/* Transition from SelectionDone to PricePayed */
					/* Action code for transition  */
					displayValue(insertedSum);


					/* OnEntry code of state PricePayed */
					releaseSelectedProduct();
					printf("VendingMachine::  Enter PricePayed\n");

					/* adjust state variables  */
					instanceVar->stateVar = PricePayed;
				}else if(insertedSum>price){
					/* Transition from SelectionDone to PricePayed */
					/* Action code for transition  */
					displayValue(insertedSum);
					releaseCoins(insertedSum-price);


					/* OnEntry code of state PricePayed */
					releaseSelectedProduct();
					printf("VendingMachine::  Enter PricePayed\n");

					/* adjust state variables  */
					instanceVar->stateVar = PricePayed;
				}else{
					/* Transition from SelectionDone to SelectionDone */
					/* Action code for transition  */
					displayValue(insertedSum);


					/* OnEntry code of state SelectionDone */
					printf("VendingMachine::  Enter SelectionDone\n");

					/* adjust state variables  */
					instanceVar->stateVar = SelectionDone;
				} /*end of event selection */
			}else if(msg==(VENDING_MACHINE_EVENT_T)evUnknownCoin){
				/* OnExit code of state SelectionDone */
				insertedSum+=getCoinValue();

				/* Transition from SelectionDone to SelectionDone */
				/* Action code for transition  */
				printf("\nUnknown coin\n");


				/* OnEntry code of state SelectionDone */
				printf("VendingMachine::  Enter SelectionDone\n");

				/* adjust state variables  */
				instanceVar->stateVar = SelectionDone;
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case SelectionDone  */

		case PricePayed:
			if(msg==(VENDING_MACHINE_EVENT_T)evReleaseDone){

				if(isEmpty()){
					/* Transition from PricePayed to FinalState0 */
					/* Action code for transition  */
					
					printf("VendingMachine:: Sold out!\n");



					/* adjust state variables  */
					instanceVar->stateVar = FinalState0;
				}else{
					/* Transition from PricePayed to WaitForSelection */
					/* OnEntry code of state WaitForSelection */
					price=0;
					insertedSum=0;
					displayValue(0);
					printf("VendingMachine::  Enter WaitForSelection\n");

					/* adjust state variables  */
					instanceVar->stateVar = WaitForSelection;
				} /*end of event selection */
			}else{
				/* Intentionally left blank */
			} /*end of event selection */
		break; /* end of case PricePayed  */

		case FinalState0:
		break; /* end of case FinalState0  */

		default:
			/* Intentionally left blank */
		break;
	} /* end switch stateVar_root */
}

/* Implementation of the reset machine function */
void vending_machineResetMachine(VENDING_MACHINE_INSTANCEDATA_T * const instanceVar){
	instanceVar->waitforselectionEntry = 1U;
	instanceVar->stateVar = WaitForSelection;
	
}

// Helper(s) to find out if the machine is in a certain state

uint8_t vending_machineIsInWaitForSelection(const VENDING_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== WaitForSelection)) ? (1U) : (0U));
}

uint8_t vending_machineIsInPricePayed(const VENDING_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== PricePayed)) ? (1U) : (0U));
}

uint8_t vending_machineIsInSelectionDone(const VENDING_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== SelectionDone)) ? (1U) : (0U));
}

uint8_t vending_machineIsInFinalState0(const VENDING_MACHINE_INSTANCEDATA_T * const instanceVar){
	return(((instanceVar->stateVar== FinalState0)) ? (1U) : (0U));
}


// Helper to get id of innermost active state
VENDING_MACHINE_STATES_T vending_machineGetInnermostActiveState(const VENDING_MACHINE_INSTANCEDATA_T * const instanceVar){
	if(vending_machineIsInFinalState0(instanceVar)){
		return FinalState0;
	}else if(vending_machineIsInSelectionDone(instanceVar)){
		return SelectionDone;
	}else if(vending_machineIsInPricePayed(instanceVar)){
		return PricePayed;
	}else if(vending_machineIsInWaitForSelection(instanceVar)){
		return WaitForSelection;
	}else{
		return __VENDING_MACHINE_LAST_STATE__;
	}
}

