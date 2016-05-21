/* (C) Your company*//* Command line options: -l cx -v -p ASTAH -o product_store_sm -t machine:product_store_sm:sm vending_machine.asta   */
/* This file is generated from vending_machine.asta - do not edit manually  */
/* Generated on: Sat May 21 23:01:44 CEST 2016 / version 3.6.14 */




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
