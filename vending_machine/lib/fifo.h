/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden.
   
   The ringbuf is a fifo like buffer. It serves as an event queue to 
   decouple event source and event consumer and preserve event order.

   There is the fifoPutHead() function to put priority events (e.g.
   emergency stop or the like) to the front of the queue. This means
   it will be returned first when a user calls fifoGet().

   Code inspired by http://www.mikrocontroller.net/articles/FIFO
 */

#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdint.h>
#include <stdbool.h>

#define QUEUE_EMPTY 1

typedef struct Buffer {
    uint8_t *data;
    uint8_t mask; // size - 1
    uint8_t read; // index of oldest element
    uint8_t write; // index of field to write to
} FIFO_T;
    
void fifoInit(FIFO_T * const buffer, uint8_t  * const pByte, uint8_t size);
bool fifoPut(FIFO_T * const buffer, uint8_t byte);
bool fifoGet(FIFO_T  * const buffer, uint8_t * const pByte);
bool fifoPutHead(FIFO_T * const buffer,  uint8_t byte);
bool fifoIsEmpty(const FIFO_T * const buffer);

#endif /* FIFO_H */
