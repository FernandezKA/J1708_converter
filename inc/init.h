#ifndef _init_h_
#define _init_h_
#include "stm8s_conf.h"
//User definition
#define Fclk 16000000UL
#define BAUD 9600UL
//Function declaration
void GPIO_Config(void);
void CLK_Config(void);
void UART_Config(void);
#endif