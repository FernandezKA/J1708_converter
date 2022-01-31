#include "stm8s_conf.h"
//User variables
//The receive and transmit parsed package
j1708 jReceiveStr;
j1708 jTransmitStr;
//Timing variable
volatile uint8_t u8TimePrior = 0x20;
uint16_t u16cTime = 0x00U;
enum TSTATE tState = wait;
//Receive data from j1708
uint8_t u8WithoutTimeOut = 0x00;
bool isTimeout = FALSE;
FIFO j1708FIFO;
//User function declaration
j1708 jReceive(struct FIFO_STR *fReceive)
{
  j1708 rStruct;
  //Get data size of buffer
  rStruct.length = fReceive->u8Head - fReceive->u8Tail - 0x02;

  rStruct.MID = Pull(fReceive);

  for (uint8_t i = 0; i < rStruct.length; ++i)
  {
    rStruct.data[i] = Pull(fReceive);
  }

  rStruct.CRC = Pull(fReceive);
  return rStruct;
}
//j1708 transmit packet
void jTransmit(volatile j1708 *tStruct, uint8_t u8Priority)
{
  u8TimePrior = 8 + 2 * u8Priority;
  uint8_t u8PackCnt = 0x00;
  while (u8PackCnt < tStruct->length)
  {
    while ((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE)
    {
      asm("nop");
    } //Wait empty buffer
    if (u8PackCnt == 0x00)
    { //MID
      UART1->DR = tStruct->MID;
    }
    else if (u8PackCnt == tStruct->length)
    { //CRC
      UART1->DR = tStruct->CRC;
    }
    else
    { //Data
      UART1->DR = tStruct->data[u8PackCnt - 0x01];
    }
    ++u8PackCnt;
  }
  while ((UART1->SR & UART1_SR_TXE) != UART1_SR_TXE)
  {
    asm("nop");
  }
  tState = wait;
  u16cTime = 0x00;
}
//IRQ Handler for TIM1
void Tim1_Handler(enum TSTATE *cState, uint16_t *cTime)
{
  if (*cTime == 0xFFFFUL)
  {
    *cTime = 11U;
  }
  else
  {
    *cTime = (*cTime) + 0x01U; //Increment size of time bytes
  }
  if (*cTime > 2U && *cTime < 10)
  {
    *cState = stop_package;
  }
  else if ((*cTime) > u8TimePrior)
  {
    *cState = free_bus;
    //GPIOB->ODR&=~(1<<5);
  }
}
