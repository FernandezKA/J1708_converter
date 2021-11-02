#include "stm8s_conf.h"
//User variables
volatile j1708 jReceiveStr;
volatile j1708 jTransmitStr;
enum BUS_STATE bus_state;
enum TSTATE tState;
uint16_t ui16cTime;
//User function declaration
//j1708 receive packet
j1708 jReceive(enum Receive_FSM* eFSM){
  j1708 rStruct;
  switch(*eFSM){
  case MID:
      jReceiveStr.MID = UART1->DR;
      break;
  case DATA:
      
      break;
  case CRC:
      
      break;
    default:
    
    break;
  }
  return rStruct;
}
//j1708 transmit packet
void jTransmit(j1708 tStruct){
  
}