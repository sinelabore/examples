/**
 *
 * (C) Peter Mueller, www.sinelabore.com
 * This file contains defines a simple timer functionality used in our state
 * machine demo programs.
 *
 * Generally the timers work as follows:
 *  - the tick function must be called cyclically e.g. in a irq handler
 *  - timers works on tick basis. There is no absolute time.
 *  - if there is an active timer and the defined numbers of ticks have 
 *    elapsed, a previously provided timeout event
 *    will be stored in a queue for further processing by the state machine.
 *  - the handler calling the tick() function get a flag that indicates 
 *    that at least one timer fired. This can be used to call the state
 *    machines checking if there is a message in a queue.
 */   

#include <stdint.h>
#include "fifo.h"
   
#ifndef __TIMER_H_
#define __TIMER_H_

#define T1S 10
#define T3S 30

// Defines for the timer
typedef enum {
	TIMER_OFF=0U,
	TIMER_ON=1U,
	TIMER_PAUSE=2U,
	TIMER_UNUSED=3U,
	TIMER_UNKNOWN=4U,
} TIMER_STATE_T;

// init memory reserved for timers
void timerInit(void);

// Create new timer and returns timer id. Provide the buffer
// the timeout event should be enqueued to.
uint8_t timerCreate(FIFO_T *const fifo, uint8_t eventId,bool kind);

// starts timer
// Returns either ON if started or OFF if not started (e.g. preset=0)
TIMER_STATE_T timerStart(uint8_t t_id, uint16_t ticks);

// stops timer
void timerStop(uint8_t t_id);

// either returns ON or OFF or PAUSE
TIMER_STATE_T timerStatus(uint8_t t_id);

// Timer function to be called from the timer irq handler.
// If at least on timer fired the function returns 1. Otherwise 0.
bool tick(void);

// pause timer
void timerPause(uint8_t t_id);

// resume timer
void timerCont(uint8_t t_id);

// Not yet implemented
void timerErase(uint8_t t_id);

#endif
