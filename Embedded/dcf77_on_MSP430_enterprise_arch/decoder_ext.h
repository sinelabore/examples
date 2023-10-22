/*
 * (c) Peter Mueller, 2023
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -t Model:Class Model:decoder -p EA -o decoder decoder.xml   */
/* This file is generated from decoder.xml - do not edit manually  */
/* Generated on: Wed Dec 28 17:29:00 CET 2022 / Version 5.5.5.6 */




#ifndef DECODER_EXT_H 
#define DECODER_EXT_H 

/*Events which can be sent to the state-machine  */
typedef enum
{
	evTimingError=0U,
	evMinStart,
	evTick,
	DECODER_NO_MSG
} DECODER_EVENT_T;


#endif
