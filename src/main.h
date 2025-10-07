// main.h
// Josh Brake
// jbrake@hmc.edu
// 10/31/22 

#ifndef MAIN_H
#define MAIN_H

#include "STM32L432KC.h"
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Custom defines
///////////////////////////////////////////////////////////////////////////////

#define LED_PIN PA5
#define ENCODERA PA1
#define ENCODERB PA2
#define DELAY_TIM TIM2
#define PPR 408  // multiplying by 4 because encodr counts 4 counters per pulse period

#endif // MAIN_H