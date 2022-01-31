#ifndef _init_h_
#define _init_h_
#include "stm8s_conf.h"
//User definition
#define SWUART_RECEIVE_USED
#define SWUART_TRANSMIT_USED
#define TRUE true
#define FALSE false
#define Fclk 16000000UL
#define BAUD 9600UL
//Function declaration
void GPIO_Config(void);
void CLK_Config(void);
void UART_Config(void);
void Tim1_Config(void);
void Tim4_Config(void);

enum CRC_MODE{
  show_invalid, 
  hide_invalid
};
extern enum CRC_MODE crc_mode;
#endif