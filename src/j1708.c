#include "stm8s_conf.h"
//User variables
volatile j1708 jReceiveStr;
volatile j1708 jTransmitStr;
volatile uint8_t u8TimePrior = 0x20;
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
void jTransmit(volatile j1708* tStruct, uint8_t u8Priority){
    u8TimePrior = 8 + 2*u8Priority;
    uint8_t u8PackCnt = 0x00;
    while(u8PackCnt < tStruct->length){
      while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE){asm("nop");}//Wait empty buffer
      if(u8PackCnt == 0x00){//MID
        UART1->DR = tStruct->MID;
      }
      else if(u8PackCnt == tStruct->length){//CRC
        UART1->DR = tStruct ->CRC;
      }
      else{//Data
        UART1->DR = tStruct->data[u8PackCnt - 0x01];
      }
           ++u8PackCnt;
    }
    while((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE){asm("nop");}
    tState = wait;
    u16cTime = 0x00;
    //GPIOB->ODR|=(1<<5);
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
    else if((*cTime) > u8TimePrior){
      *cState = free_bus;
      //GPIOB->ODR&=~(1<<5);
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