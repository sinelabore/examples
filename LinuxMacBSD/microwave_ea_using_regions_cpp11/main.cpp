/**
 * @file main.cpp
 * @author pmueller
 * @brief Microwave oven simulator
 * @version 0.1
 * @date 2023-06-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include "oven_ext.h"
#include "oven.h"
#include "my_oven.h"
#include "SafeQueue.hpp"
#include "SimpleTimer.hpp"


extern SafeQueue<OVEN_EVENT_T> q;
// SimpleTimer<Events> t1(q), t2(q);


std::thread eventHandlerThread() {
  my_oven* machine = new my_oven();
  machine->initialize();
  while (true) {
    OVEN_EVENT_T msg;
    q.waitAndPop(msg);
    // std::cout << static_cast<int>(msg) << std::endl;
    machine->processEvent(msg);
  }
}


int main() {
  // start event handler thread
  std::thread tid = std::thread(eventHandlerThread);

  std::cout << "---------" << std::endl;
  std::cout << "Type in one of the following characters followed by return" << std::endl;
  std::cout << "to stimulate the state machine." << std::endl;
  std::cout << "+ increase cooking time by 1s" << std::endl;
  std::cout << "- decrease cooking time by 1s" << std::endl;
  std::cout << "P set high power mode" << std::endl;
  std::cout << "p set low power mode" << std::endl;
  std::cout << "c close the door" << std::endl;
  std::cout << "o open the door" << std::endl;
  std::cout << "It is also possible to give more than one command e.g. ++++c↵" << std::endl;
  std::cout << "---------" << std::endl;

  bool run = true;
  do {
    OVEN_EVENT_T msg;
    std::string str;
    str = std::cin.get();
    for (char& c : str) {
      switch (c) {
        case '+':
          msg = OVEN_EVENT_T::evInc;
        break;

        case '-':
          msg = OVEN_EVENT_T::evDec;
        break;

        case 'c':
          msg = OVEN_EVENT_T::evDoorClosed;
        break;

        case 'o':
          msg = OVEN_EVENT_T::evDoorOpen;
        break;

        case 'p':
          msg = OVEN_EVENT_T::evPwrLow;
        break;

        case 'P':
          msg = OVEN_EVENT_T::evPwrHigh;
        break;

        case 'q':
          run = false;
          std::cout << "end simulation" << std::endl;
        break;

        default:
          msg = OVEN_EVENT_T::OVEN_NO_MSG;
        break;
      }

      q.push(msg);

    }
  }while(run);

  // not elegant, in real app threads must be killed here.

  return 0;
}

