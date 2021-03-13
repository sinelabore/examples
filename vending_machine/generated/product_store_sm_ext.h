/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -l cx -v -p ASTAH -o product_store_sm -t machine:product_store_sm:sm ../vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sat Mar 13 15:41:53 CET 2021 / Version 5.0.6b1 */




#ifndef __PRODUCT_STORE_SM_EXT_H__ 
#define __PRODUCT_STORE_SM_EXT_H__ 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evReleaseSelectedProduct=0U,
	evTimeout,
	PRODUCT_STORE_SM_NO_MSG
} PRODUCT_STORE_SM_EVENT_T;


#endif
