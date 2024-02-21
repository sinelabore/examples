/**
 * @file ovenbase.h
 * @author your name (you@domain.com)
 * @brief  baseclass of generated oven state machine implementing some supporting methods
 * @version 0.1
 * @date 2023-06-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __OVENBASE__

#include <stdint.h>
#include <iostream>

// Defines for Power Control
enum class T_PWR : uint8_t {
  PWR_LOW,
  PWR_HIGH,
} ;

// Defines for Light Commands
enum class T_LIGHT : uint8_t {
  L_ON,
  L_OFF,
};

class ovenbase {

 protected:
  // just for demo
  uint8_t hardwareTest(void) {
    std::cout << "Hardwaretest ok" << std::endl;
    return 0U;
  }


  void ovenOff() {
    std::cout << "Oven off" << std::endl;
  }

  void ovenOn() {
    std::cout << "Oven On" << std::endl;
  }

  void ovenSetPwr(T_PWR pwr_in) {
    if (pwr_in == T_PWR::PWR_LOW)
        std::cout << "Oven::set to 'PWR_LOW'" << std::endl;
    else if (pwr_in == T_PWR::PWR_HIGH)
      std::cout << "Oven::set to 'PWR_HIGH'" << std::endl;
  }


  void switchLight(T_LIGHT lcmd) {
    if (lcmd == T_LIGHT::L_OFF)
      std::cout << "Light switched 'OFF'" << std::endl;
    else if (lcmd == T_LIGHT::L_ON)
      std::cout << "Light switched 'ON'" << std::endl;
  }
};

#endif
