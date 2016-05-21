/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden.
 */

#ifdef __MSP430__
#include <msp430.h>
#define DISABLE_IRQ __disable_interrupt()
#define ENABLE_IRQ  __enable_interrupt()
#else
#define DISABLE_IRQ
#define ENABLE_IRQ
#endif

#include <stdint.h>

#include "timer.h"
#include "fifo.h"

typedef struct {
  TIMER_STATE_T timerStatus;
  bool cyclic;
  uint16_t preset;
  uint16_t elapsedTicksSinceStart;
  FIFO_T *rbuf;
  uint16_t evTimeout;
}TIMER_T;

#define MAX_TIMERS 3U
#define NO_TIMERS_LEFT 127U

TIMER_T timers[MAX_TIMERS];

void timerInit(void){
	uint8_t n;
	for(n=0U; n<MAX_TIMERS;n++){
		timers[n].timerStatus=TIMER_UNUSED;
		timers[n].cyclic=false;
		timers[n].preset=0U;
		timers[n].elapsedTicksSinceStart=0U;
		timers[n].rbuf=0;
		timers[n].evTimeout=0U;
	}
}


/**
 * This function has to be called from the timer irq to
 * cyclically check if a timeout has happened.
 */
bool tick(void){
  bool timeoutAvailable=false;
  
  // Iterate over the timers and enque
  // timer events once a timeout has happend.
  uint8_t i;
  for(i=0U; i<MAX_TIMERS; i++){
  
    if(timers[i].timerStatus==TIMER_ON){
      
      timers[i].elapsedTicksSinceStart++;
                  
      if(timers[i].elapsedTicksSinceStart==timers[i].preset){

    	if(timers[i].cyclic==true){
    		timers[i].elapsedTicksSinceStart = 0U;
    	}else{
            // timeout time elapsed.
            timers[i].timerStatus=TIMER_OFF; // stop timer
    	}

    	DISABLE_IRQ;
        fifoPut(timers[i].rbuf, timers[i].evTimeout); // enqueue timeout event
        ENABLE_IRQ;
        timeoutAvailable=true;
      }
    }
  }
  
  return timeoutAvailable;
}

//
// Functions for all timers
//

uint8_t timerCreate(FIFO_T *const fifo, uint8_t eventId, bool kind){
  uint8_t loopCounter=0U;
  
  while(loopCounter<MAX_TIMERS){
    if((timers[loopCounter]).timerStatus==TIMER_UNUSED){
    	(timers[loopCounter]).cyclic=kind;
        (timers[loopCounter]).timerStatus = TIMER_OFF;
        (timers[loopCounter]).rbuf = fifo;
        (timers[loopCounter]).evTimeout=eventId;
        return loopCounter;
    }
    loopCounter++;
  }
  return NO_TIMERS_LEFT; // no timers left
}

void timerErase(uint8_t t_id){
  // not jet impl.
}

//
// Timer functions for a single timer
//

void timerPause(uint8_t t_id){
  if(t_id>(MAX_TIMERS-1U)) return;
  timers[t_id].timerStatus=TIMER_PAUSE;
}

void timerCont(uint8_t t_id){
  if(t_id>(MAX_TIMERS-1U)) return;
  timers[t_id].timerStatus=TIMER_ON;
}

void timerClear(uint8_t t_id){
  if(t_id>(MAX_TIMERS-1U)) return;
  timers[t_id].preset=0U;
}

TIMER_STATE_T timerStart(uint8_t t_id, uint16_t ticks){
  if(t_id>MAX_TIMERS) return TIMER_UNKNOWN;

  timers[t_id].preset=ticks;
  timers[t_id].elapsedTicksSinceStart = 0U;
  timers[t_id].timerStatus = TIMER_ON;
  return TIMER_ON;
}

void timerStop(uint8_t t_id){
  if(t_id>(MAX_TIMERS-1U)) return;

  timers[t_id].timerStatus=TIMER_OFF;
}

TIMER_STATE_T timerStatus(uint8_t t_id){
	if(t_id>(MAX_TIMERS-1U)) return TIMER_UNKNOWN;

	return timers[t_id].timerStatus;
}

