#ifndef _j1708_h_
#define _j1708_h_
//User include 
#include "stm8s_conf.h"
#include "fifo.h"
//
//enum  Receive_FSM{
//  MID = 0, 
//  DATA, 
//  CRC
//};
//extern enum Receive_FSM R_FSM;
extern uint8_t u8WithoutTimeOut;
extern bool isTimeout;
extern struct FIFO_STR j1708FIFO;
enum TSTATE{
  wait = 0, 
  stop_package,
  free_bus
};
extern enum TSTATE tState;
extern uint16_t u16cTime;
//Struct for j1708 packet
struct J1708{
  uint8_t MID;
  uint8_t data[64U];
  uint8_t CRC;
  uint8_t length;
};
typedef struct J1708 j1708;
extern bool shMCRC;

//User variables
extern j1708 jReceiveStr;
extern j1708 jTransmitStr;
extern volatile uint8_t u8TimePrior;
//User function definition
//This function for receieve data from j1708 bus
j1708 jReceive(struct FIFO_STR* fReceive);
//This function for transmit data from j1708 bus
void jTransmit(j1708* tStruct, uint8_t u8Priority);
//This function is IRQ handler for Tim1 
void Tim1_Handler(enum TSTATE* cState, uint16_t* cTime);
//This function is IRQ handler for UART1
//void UART1_Rx_Handler(uint16_t* cTime, enum TSTATE* cState,enum Receive_FSM* R_FSM,volatile j1708* RPack);
#endif