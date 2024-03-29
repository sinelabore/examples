
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <memory>
#include <iostream>
#include "oven_ext.h"
#include "oven.h"
#include "oven_hlp.h"


void init_keyboard(void);
void close_keyboard(void);
int kbhit(void);
int readch(void);

static struct termios init_tio;
static struct timespec tv = {0, 1000*1000*100};  // ~100ms

// parameter
T_PWR pwr = PWR_LOW;


// message to state machine
OVEN_EVENT_T msg;

void init_keyboard() {
    tcgetattr(0, &init_tio);
}
void close_keyboard() {
    tcsetattr(0, TCSANOW, &init_tio);
}

int kbhit() {
    struct termios tio;
    struct timeval tv;
    fd_set rfds;
    // set up terminal
    memcpy(&tio, &init_tio, sizeof(struct termios));
    tio.c_lflag &= ~(ICANON);
    tcsetattr(0, TCSANOW, &tio);
    // do not wait
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_usec = 0;
    tv.tv_sec  = 0;
    select(1, &rfds, NULL, NULL, &tv);
    // back to initial terminal mode
    tcsetattr(0, TCSANOW, &init_tio);
    return (FD_ISSET(0, &rfds)?1:0);
}

int readch() {
    int ch;
    struct termios tio;
    // set up terminal
    memcpy(&tio, &init_tio, sizeof(struct termios));
    tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tio);
    // input key
    read(0, &ch, 1);
    // back to initial terminal mode
    tcsetattr(0, TCSANOW, &init_tio);
    return ch;
}

void usage(void) {
  std::cout << "Unknown key. Use + for inc; - for dec; "
            << "o for open and c for close.\n";
  std::cout << "Use 'p' to set medium power; 'P' to set high power\n";
}

int main(void) {

  unsigned char timer_status;
  oven* machine = new oven();
  machine->initialize();

  //
  // interactive mode
  //
  init_keyboard();
  usage();
  do {
    nanosleep(&tv, 0);
    timer_status = tick();

    if (timer_status) {
      msg = evTimeout;
    } else if (kbhit()) {
      char ch = readch();
      if (ch == '+') {
        msg = evInc;
      } else if (ch == '-') {
        msg = evDec;
      } else if (ch == 'c') {
        msg = evDoorClosed;
      } else if (ch == 'o') {
        msg = evDoorOpen;
      } else if (ch == 'q') {
        close_keyboard();
        exit(0);
      } else if (ch == 'P') {
        pwr = PWR_HIGH;
        msg = evPwrHigh;
      } else if (ch == 'p') {
        pwr = PWR_LOW;
        msg = evPwrLow;
      } else {
        usage();
        msg = OVEN_NO_MSG;
      }
    } else {
      msg = OVEN_NO_MSG;
    }

    machine->processEvent(msg);

  } while (1);
  return 0;
}
