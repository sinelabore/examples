/* (c) Peter Mueller, 2012 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -d -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Wed Feb 27 18:10:10 CET 2013 / version 3.11 */



#include "oven_validate.h"

#define SIZE_TRANSITION_TABLE (10U)
#define TRANSITION(from,to)  (uint16_t)((from * SIZE_TRANSITION_TABLE) + to)

/* Hash table of valid transitions */
static const uint16_t valid_transitions[] = {
	11U , /* Active->Active */
	1U , /* Inactive->Active */
	23U , /* LightOff->LightOn */
	32U , /* LightOn->LightOff */
	54U , /* HighPower->LowPower */
	45U , /* LowPower->HighPower */
	86U , /* Cooking->RadiatorOff */
	87U , /* Cooking->CookingPause */
	78U , /* CookingPause->Cooking */
	68U , /* RadiatorOff->Cooking */
};


uint8_t ovenValidate(uint16_t from, uint16_t to){

	uint16_t hash = TRANSITION(from,to);
	uint16_t x;

	for(x = 0U; x < SIZE_TRANSITION_TABLE; x ++)
	{
		if(valid_transitions[x] == hash)
		{
			return 0U;
		}
	}
	/* return an error */
	return 1U;
}
