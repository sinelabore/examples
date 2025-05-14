/* (c) Sinelabore Software Tools GmbH - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden. Generator running in demo mode!
   Please purchase a license if you want to use this software in projects.
 */

/* Command line options: -Trace -p CADIFRA -o complex complex.cdd   */
/* This file is generated from complex_trace - do not edit manually  */
/* Generated on: 6.5.3 */ 


#ifndef COMPLEX_TRACE_H
#define COMPLEX_TRACE_H

#include <stdint.h> 

/*Simulation strings which can be sent to the state-machine statediagram.simulator via UDP */
/*or to print which event triggered a state change in trace code.  */
#define COMPLEX_TRACE_EVENT_LEN (18U)

void complexTraceEvent(uint16_t inst_id, uint16_t evt);


#endif
