/* Copyright Peter Mueller 2023*/

#include <stdio.h>
#include <curses.h>
#include <stdint.h>
#include "mydefs.h"
#include "oven_hlp.h"

static T_TIMER_STATE timerStatus;
static uint16_t tickval = 0;
static uint8_t presetval = 0;

extern T_PWR pwr;
extern WINDOW *logwin;

//
// Timer functions
//

// tick is called every 100ms
uint8_t tick() {
  if (timerStatus == ON) {
    if (tickval < presetval*10) {
      tickval++;
      wprintw(logwin, "Timer %ds left\n", presetval - (tickval/10));
    }
    if (tickval == presetval*10) {
      wprintw(logwin, "Timer::timeout\n");
      timerStatus = OFF;
      return 1;
    }
  }
  return 0;
}

void timer_pause(void) {
  timerStatus = PAUSE;
}

void timer_cont(void) {
  timerStatus = ON;
}

void timer_clear(void) {
  presetval = 0;
}

unsigned char timer_preset() {
  return presetval;
}

T_TIMER_STATE timer_start() {
  wprintw(logwin, "Timer::start\n");

  if (presetval !=0) {
    tickval = 0;
    timerStatus = ON;
    return ON;
  } else {
    timerStatus = OFF;
    return OFF;
  }
}

void timer_stop(void) {
  wprintw(logwin, "Timer::stop\n");
  timerStatus = OFF;
}

T_TIMER_STATE timer_status(void) {
  return timerStatus;
}

void timer_inc(void) {
  if (presetval < 60) {
    presetval++;
    wprintw(logwin, "Timer::inc -> preset=%d\n", presetval);
  }
}

void timer_dec(void) {
  if (presetval > 0) {
    presetval--;
    wprintw(logwin, "Timer::dec -> preset=%d\n", presetval);
  }
}

// Pwr Functions

void ovenSetPwr(T_PWR pwr_in) {
  if (pwr_in == PWR_LOW)
    wprintw(logwin, "Oven::set to 'PWR_LOW'\n");
  else if (pwr_in == PWR_HIGH)
    wprintw(logwin, "Oven::set to 'PWR_HIGH'\n");
}

void ovenOff(void) {
  wprintw(logwin, "Oven::off\n");
}

void ovenOn(void) {
  wprintw(logwin, "Oven::on\n");
}

