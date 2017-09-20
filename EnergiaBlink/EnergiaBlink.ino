/*
 Energia (= Arduino for Texas Instruments processors) demo

 The program demonstrates how to use code generated from Sinelabore with Energia.

 When the program starts the LED1 blinks with 1s cycle time
 S2 allows to switch to a faster cycle time and back again
 S1 allows to enter a state where the LED1 is on or off (see state diagram).

 This demo shows that is is very easy to realize complex behaviour 
 which is easy to understand on state diagram level - but not so easy
 anymore on code level. AND: if you changes something - just regenerate
 the state machine code - no hand coding needed!!!
 
*/

#include "Statemachine_ext.h"
#include "Statemachine.h"

extern "C"
{
#include "timer.h"
#include "fifo.h"
}

#define BUTTON2 PUSH2
#define BUTTON1 PUSH1

//see pins_energia.h for more LED definitions
//#define LED GREEN_LED

// generated state machine
Statemachine myGeneratedSM;

// library objects for timing and fifo
FIFO_T fifo; // fifo object
uint8_t fifoRawMem[32]; // memory to store fifo msgs in
uint8_t timerId;

// irq handler for button
void Button2FallingIrq() {
  static unsigned long lastButtonIrqTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastButtonIrqTime > 100){
    fifoPut(&fifo, evButton2);
  }
  lastButtonIrqTime = currentMillis;
}

void Button1FallingIrq() {
  static unsigned long lastButtonIrqTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastButtonIrqTime > 100){
    fifoPut(&fifo, evButton1);
  }
  lastButtonIrqTime = currentMillis;
}

// othr vars
uint16_t previousMillis = 0;


// the setup routine runs once when you press reset:
void setup() { 

  Serial.begin(115200); // for debugging
                
  // initialize the digital pin as an output.
  pinMode(RED_LED, OUTPUT);
  pinMode(BUTTON2, INPUT_PULLUP);
  attachInterrupt(BUTTON2, Button2FallingIrq, FALLING);  // interrupt 
  pinMode(BUTTON1, INPUT_PULLUP);
  attachInterrupt(BUTTON1, Button1FallingIrq, FALLING);  // interrupt   

  // init fifo and timer
  fifoInit(&fifo, fifoRawMem, 32 /* max number of events*/);
  timerInit();

  myGeneratedSM.initialize();
}

// main loop
void loop() {

  delay(100); 

  // call tick() every 100ms to operate timer service
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > 100) {
    previousMillis = currentMillis;
    tick();
  }

  // check if there is an entry in the event queue. If yes, call state machine with event
  boolean fifoEmpty = fifoIsEmpty(&fifo);
  if (!fifoEmpty) {
    uint8_t evt;

    fifoGet(&fifo, &evt);
    if(evt!=evTimeout) Serial.println(myGeneratedSM.getNameByEvent((STATEMACHINE_EVENT_T)evt)); // debug
    myGeneratedSM.processEvent((STATEMACHINE_EVENT_T)evt);
  } else {
    myGeneratedSM.processEvent(STATEMACHINE_NO_MSG);
  }

  // debugging: print current state the machine is in
  unsigned short state = myGeneratedSM.getInnermostActiveState();
  Serial.println(myGeneratedSM.getNameByState(state));
}
