/**
 * @file my_oven.h
 * @author pmueller
 * @brief Subclass of generated oven class overwriting the trace method for debug purposes
 * @version 0.1
 * @date 2023-06-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "oven.h"

class my_oven : public oven{
 public:

  // Called each time from generated state machine
  // code when a state transtion happens
  void validationHandler(const States from, const States to) const {
    std::cout << "Transition from state: " << getNameByState(from) << " to state " << getNameByState(to) << std::endl;
  }
};
