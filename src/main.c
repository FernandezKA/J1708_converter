#include "stm8s_conf.h"
#include "init.h"
#include "softuart.h"
#include "fifo.h"
#include "communication.h"
//User defines
#define enIRQ asm("rim")
#define disIRQ asm("sim")
//Function definition
static void SysInit(void);
static inline void PrintHelp(void);
static inline void SendArray(uint8_t *pData, uint8_t Size);
//static inline uint8_t GetCRC(j1708 *Struct);
bool shMCRC = false;
//Main section
uint8_t RxBuf;
void main(void)
{
  SysInit();
  PrintHelp();
  //UART1->CR2&=~UART1_CR2_REN;//ONLY FOR DEBUG!!!
  //uint8_t swRequest = 0x00;
  //uint8_t j1708DataCnt = 0x00;
  for (;;)
  {
    
    
    if (j1708FIFO.isEmpty == FALSE)
    { //Check for j1708 end of transaction
      asm("nop");
      if (tState == free_bus)
      {                                     //Send j1708 packet at RS232
        jReceiveStr = jReceive(&j1708FIFO); //Get parse recieved ring buffer
        ReflectPacket(From_j1708_to_RS232, &jReceiveStr, 0);
      }
    }
    //Receive data from RS232
    if (test_status(receive_buffer_full) == receive_buffer_full)
    {                       //Receive data from software UART
      asm("sim");
      uart_read(&RxBuf);    //Load data into buffer uart_sw
      if(RxBuf == 0xFF){//CRC mode view
        if (shMCRC){
          shMCRC = false;
          SendArray((uint8_t*)"Hide packets with mistakes\r\n", 28);
        }
        else{
          shMCRC = true;
          SendArray((uint8_t*)"Show packets with mistakes\r\n", 28);
        }
      }
//      else{
//      switch(swRequest){
//      case 0x00://MID
//        jTransmitStr.MID = RxBuf;
//        ++swRequest;
//        break;
//      case 0x01://SIZE
//        jTransmitStr.length = RxBuf;
//        ++swRequest;
//        break;
//      case 0x02://Data 
//        if(j1708DataCnt < jTransmitStr.length - 1){
//          jTransmitStr.data[j1708DataCnt] = RxBuf;
//          ++j1708DataCnt;
//        }
//        else{
//          jTransmitStr.data[j1708DataCnt] = RxBuf;
//          j1708DataCnt = 0x00;
//          jTransmitStr.CRC = GetCRC(&jTransmitStr);
//          ++swRequest;
//        }
//        break;
//      case 0x03://Calc. CRC
//          jTransmit(&jTransmitStr, RxBuf);
//          swRequest = 0x00;
//        break;
//      }
//      asm("rim");
//    }
    }
  }
}
//Function declaration
static void SysInit(void)
{
  CLK_Config();
  GPIO_Config();
  UART_Config();
  Tim1_Config();
  Tim4_Config();
  uart_init();
  uart_receive_enable;
  enable_cc_interrupt;
  enableInterrupts();
}
//Assert failed for SPL
#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
//For print info message
static inline void PrintHelp(void)
{
  SendArray((uint8_t*)"J1708 converter. ver. 0.1 2022-02-02\n\r", 38);
  SendArray((uint8_t *)"J1708 Receiver. Only listen J1708, transmit isn't supported\n\r", 61);
  SendArray((uint8_t*) "Send 0xFF to show invalid CRC packets\n\r", 39);
}
//For send array of data to soft uart
static inline void SendArray(uint8_t *pData, uint8_t Size)
{
  asm("rim");
  for (uint8_t i = 0; i < Size; ++i)
  {
    //Wait TXE flags
    while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
    {
      asm("nop");
    }
    uart_send(*(pData + i));
  }
}
//This function calculate CRC for structure
//static inline uint8_t GetCRC(j1708 *Struct)
//{
//  uint8_t sum = Struct ->MID;
//  for(uint8_t i = 0; i < Struct->length; ++i){
////    if (sum + Struct->data[i] > 0xFF)
////            {
////                  //sum--;
////                  sum += Struct->data[i];
////            }
////            else
////            {
////                  sum += Struct->data[i];
////            }
//      sum += Struct->data[i];
//  }
//      return (sum ^ 0xFF)+1;
//}
#endif