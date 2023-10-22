/*
 * (c) Sinelabore Software Tools GmbH, 2008 - 2022
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

/* Command line options: -Trace -l cppx -p EA -t Model:implementation:oven -o oven oven.xml   */
/* This file is generated from oven_trace - do not edit manually  */
/* Generated on: Version 6.0.2.3601 */ 


#ifndef OVEN_TRACE_H
#define OVEN_TRACE_H

#include <cstdint>
#include <string>
#include <array>

/*Simulation strings which can be sent to the state-machine statediagram.simulator via UDP */
/*or to print which event triggered a state change in trace code.  */
#define OVEN_TRACE_EVENT_LEN (10U)

void ovenTraceEvent(const std::uint16_t evt);

std::array<std::string const,10U> const ovenTraceEvents {
	"evDec",
	"evInc",
	"evDoorClosed[timer.preset()>0]",
	"evDoorOpen[timer.preset()==0]",
	"evPwrLow",
	"evPwrHigh",
	"evTimeout",
	"evDoorOpen",
	"evDoorClosed",
	"__INIT__"
};

void ovenLightTraceEvent(uint16_t evt);
void ovenPowerTraceEvent(uint16_t evt);
void ovenRadioatorTraceEvent(uint16_t evt);


#endif
