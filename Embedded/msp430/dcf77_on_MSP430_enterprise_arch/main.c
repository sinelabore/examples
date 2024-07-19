/*
 * (c) Peter Mueller, 2009 
 *
 * All rights reserved. Reproduction, modification,
 * use or disclosure to third parties without express
 * authority is forbidden.
 */

#include  <msp430x12x2.h>
#include  "mydefs.h"
#include  "decoder_ext.h"
#include  "decoder.h"
//#include "io430.h"

// manual of DCF77 module:
// Low:  40-130ms
// High: 140-230ms

// irq cycle time 3.8ms
#define SHORT_MIN 0xB  // 
#define SHORT_MAX 0x22  // 
#define LONG_MIN 0x24   // 
#define LONG_MAX 0x3C   // 


unsigned char highCnt=0;
unsigned short lowCnt=0;
unsigned char highLowFlag=0;
unsigned char lowHighFlag=0;
TUSIGN8 msg;
TUSIGN8 len;
TUSIGN8 cntTimingErrors;

//TUSIGN8 buf[60]={0};
//TUSIGN8 bufIdx=0;

void main(void)
{ 
  WDTCTL = WDTPW + WDTHOLD;             // Stop WDT
  TACTL = TASSEL0 + TACLR;              // ACLK, clear TAR
  CCTL0 = CCIE;                         // CCR0 interrupt enabled
  CCR0 = 125;
  TACTL |= MC0; 
  
  P1DIR = 0x0;                          // by default all P1 as inputs
  P1DIR = 0x6;                          // P1.1/P1.2 is outputs to signal DCF77 signal on LED
  _EINT();                              // Enable interrupts
  
  cntTimingErrors=0;
 
  while(1) {
    _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/ interr until char RXed    
    __no_operation();
    
  }
}

// Called when valid time/date info was received
// Not implemented
TUSIGN8 setClock(void){

  // Set LCD clock etc here.
  // Check parity flags before ...
  
  return 0;
}


#pragma vector=TIMERA0_VECTOR 
__interrupt void Timer_A0(void){
  P1OUT |= 0x04; // to test runtime of irq

  msg=DECODER_NO_MSG;  
  if((P1IN&0x01)==1){
    P1OUT|=0x02;
    highCnt++;
    highLowFlag=1;
    
    if(lowHighFlag){
      if(lowCnt>380){
         msg=evMinStart;
 //        bufIdx=0;
      }
      lowHighFlag=0;
    }
    
  }else{
    if(highLowFlag){ // do this only at falling edge
      lowCnt=0;
      lowHighFlag=1;
      highLowFlag=0;
      
      // decide upon event
      if((highCnt>=SHORT_MIN) && (highCnt<=SHORT_MAX)){
        len=SHORT;
        msg=evTick;
        //buf[bufIdx++]=SHORT;
      }else if((highCnt>=LONG_MIN) && (highCnt<=LONG_MAX)){
        len=LONG;
        msg=evTick;
        //buf[bufIdx++]=LONG;
      }else{
        msg=evTimingError;
        cntTimingErrors++;
        //bufIdx=0;
      }
      highCnt=0;
    }else{
      lowCnt++;
    }
    
    P1OUT&=0xfD;
  }
  
  // call state machine with event (if any)
  if(msg!=DECODER_NO_MSG)
    decoder();

  P1OUT&=0xfb; // to test runtime of irq
}
