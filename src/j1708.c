#include "stm8s_conf.h"
//User variables
volatile j1708 jReceiveStr;
volatile j1708 jTransmitStr;
enum BUS_STATE bus_state;
enum TSTATE tState;
uint16_t u16cTime;
static uint8_t u8DataCounter = 0x00;
enum Receive_FSM R_FSM;
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
bool jTransmit(volatile j1708* tStruct){
  bool isSuccess = FALSE;
  if(tState == free_bus){
    uint8_t u8PackCnt = 0x00;
    while(u8PackCnt < 0x17U){
      while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE){asm("nop");}//Wait empty buffer
      if(u8PackCnt == 0x00){//MID
        UART1->DR = tStruct->MID;
      }
      else if(u8PackCnt == 0x17U){//CRC
        UART1->DR = tStruct ->CRC;
      }
      else{//Data
        UART1->DR = tStruct->data[u8PackCnt - 0x01];
      }
           ++u8PackCnt;
    }
    isSuccess = TRUE;
  }
  else{
    isSuccess = FALSE;
  }
  return isSuccess;
}
//IRQ Handler for TIM1
  void Tim1_Handler(enum TSTATE* cState, uint16_t* cTime){
    if(*cTime == 0xFFFFUL){
     *cTime = 11U; 
    }
    else{
    *cTime = (*cTime)+0x01U;//Increment size of time bytes
    }
    if(*cTime > 2U && *cTime < 10){
      *cState = stop_package;
    }
    else if((*cTime) > 10U){
      *cState = free_bus;
    }
  }
//IRQ UART
void UART1_Rx_Handler(uint16_t* cTime, enum TSTATE* cState, enum Receive_FSM* R_FSM, volatile j1708* RPack){
  *cTime = 0x00;//Set zero time
  *cState = wait;//Set wait state
  switch(*R_FSM){
  case MID:
    RPack ->MID = UART1->DR;
    *R_FSM = DATA;
    break;
  case DATA:
    if(u8DataCounter < 21){
      RPack->data[++u8DataCounter] = UART1->DR;
    }
    else{
      u8DataCounter = 0x00U;
      *R_FSM = CRC;
    }
    break;
  case CRC:
    RPack->CRC = UART1->DR;
    *R_FSM = MID;
    break;
  default:
    
    break;
  }
}