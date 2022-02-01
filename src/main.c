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
static inline uint8_t GetCRC(j1708 *Struct);
//Main section
uint8_t RxBuf;
enum MAIN_FSM
{
  wait_mid,
  wait_size,
  wait_data,
  wait_crc,
  wait_priority,
  set_mode,
  CRC_mode
};
//static enum MAIN_FSM main_fsm;
//enum CRC_MODE crc_mode;
//static struct FIFO_STR swUART;
void main(void)
{
  SysInit();
  PrintHelp();
  UART1->CR2&=~UART1_CR2_REN;//ONLY FOR DEBUG!!!
  //swUART.isEmpty = TRUE; //A little of black magic
  //j1708FIFO.isEmpty = TRUE;
  //main_fsm = wait_mid;
  uint8_t swRequest = 0x00;
  uint8_t j1708DataCnt = 0x00;
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
      
      switch(swRequest){
      case 0x00://MID
        jTransmitStr.MID = RxBuf;
        //UART1->DR = jTransmitStr.MID;
        //uart_send(jTransmitStr.MID);
        ++swRequest;
        break;
      case 0x01://SIZE
        jTransmitStr.length = RxBuf;
        //UART1->DR = jTransmitStr.length;
        //uart_send(jTransmitStr.length);
        ++swRequest;
        break;
      case 0x02://Data 
        if(j1708DataCnt < jTransmitStr.length - 1){
          jTransmitStr.data[j1708DataCnt] = RxBuf;
          //UART1->DR = jTransmitStr.data[j1708DataCnt];
          //uart_send(jTransmitStr.data[j1708DataCnt]);
          ++j1708DataCnt;
        }
        else{
          jTransmitStr.data[j1708DataCnt] = RxBuf;
          j1708DataCnt = 0x00;
          //uart_send(jTransmitStr.data[j1708DataCnt]);
          jTransmitStr.CRC = GetCRC(&jTransmitStr);
          ++swRequest;
        }
        break;
      case 0x03://Calc. CRC
          //uart_send(RxBuf);
          jTransmit(&jTransmitStr, RxBuf);
          swRequest = 0x00;
        break;
      }
      asm("rim");
    }
    else{
      asm("nop");
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

static inline void PrintHelp(void)
{
  SendArray((uint8_t *)"For transmit data send packet J1708 with structure MID + Size + Data + priority. CRC calculate software\n\r", 106);
}

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

static inline uint8_t GetCRC(j1708 *Struct)
{
  uint16_t Sum = 0x00;
  for (uint8_t i = 0; i < Struct->length; ++i)
  {
    Sum += Struct->data[i];
  }
  uint8_t CRC = 0x00;
  CRC = (uint8_t)(Sum & 0xFF) ^ 0xFF;
  return CRC;
}
#endif