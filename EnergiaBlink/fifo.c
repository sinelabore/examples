/* (c) Peter Mueller, 2014 - All rights reserved. Reproduction, modification,
   use or disclosure to third parties without express
   authority is forbidden.
 */

#include "fifo.h"


bool fifoPut(FIFO_T *const buffer, uint8_t byte){
    bool retVal=1U;

    uint8_t next = ((buffer->write + 1U) & buffer->mask);
    if (buffer->read != next){
      buffer->data[buffer->write] = byte;
      buffer->write = next;
      retVal=0U;
  }

    return retVal;
}

bool fifoPutHead(FIFO_T *const buffer,  uint8_t byte){

  bool retVal=1U;

    uint8_t next = ((buffer->read - 1U) & buffer->mask);
    
    if(next != buffer->write){
      buffer->data[next] = byte;
      buffer->read = next;
      retVal=0U;
    }
    
    return retVal;
}

bool fifoGet(FIFO_T * const buffer, uint8_t * const pByte){
  bool retVal=1U;

    if (buffer->read != buffer->write){
      *pByte = buffer->data[buffer->read];
      buffer->read = (buffer->read+1U) & buffer->mask;
      retVal = 0U;
    }

    return retVal;
}

bool fifoIsEmpty(const FIFO_T *const buffer){
  bool retVal=1U;

    if (buffer->read != buffer->write){
      retVal = 0U;
    }
    return retVal;
}


void fifoInit(FIFO_T *const buffer, uint8_t *const pByte, uint8_t size){
    buffer->data = pByte;
    buffer->mask  = size-1U;
    buffer->read=0U;
    buffer->write = 0U;
}
