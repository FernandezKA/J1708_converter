#ifndef _init_h_
#define _init_h_
#include "stm8s.h"
#include "softuart.h"
//This function init MCU clocking
void CLK_Init();
//This function configurate GPIO
void GPIO_Init();
//This block of function configure soft uart
void TIM2_Init();

#endif