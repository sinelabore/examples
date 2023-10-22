/* (c) Peter Mueller, 2012 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -d -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven.xml - do not edit manually  */
/* Generated on: Wed Feb 27 18:10:10 CET 2013 / version 3.11 */




#include <stdint.h>

/* Prototype of the validation handler.                                         */
/* You have to implement this function yourself and define your own             */
/* reaction if the validation fails.                                            */
void ovenValidationHandler(uint16_t from, uint16_t to, uint8_t machineId);



/* Prototype of the validate function */
/* To be called by the xxxValidationHandler */
uint8_t ovenValidate(uint16_t from, uint16_t to);


