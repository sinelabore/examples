/* (C) Sinelabore Software Tools GmbH */
/* Command line options: -p ssc -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually*/
/* Generated on: Mon Apr 01 18:02:43 CEST 2024 / Version 6.3.2.3814 */


#ifndef OVEN_H
#define OVEN_H


/* Header to define simple data types. See 'AdditionalMachineInclude' */
#include <stdint.h>

/* Instance data definition of state machine */
struct OVEN_INSTANCEDATA  {
	OVEN_ENTRY_FLAG_T superEntry;
	OVEN_STATEVAR_T stateVar;
	OVEN_STATEVAR_T stateVarMainRegion;

	/* Start of user defined attributes */
	/* End of user defined attributes */

	OVEN_INST_ID_T inst_id;
} ;


#endif
