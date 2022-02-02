#ifndef _communication_h_
#define _communication_h_
//User includes
#include "init.h"
#include "softuart.h"
#include "j1708.h"
//User definition
#define wait_uart while((UART1->SR & UART1_SR_TXE) != UART1_SR1_TXE) {asm("nop");}
//User variables
enum DirectionReflect{
  From_RS232_to_j1708, 
  From_j1708_to_RS232
};
//User functions
void ReflectPacket(enum DirectionReflect Direction,struct J1708* packet, uint8_t priority);
uint8_t u8CalcCRC(j1708* packet);
#endif