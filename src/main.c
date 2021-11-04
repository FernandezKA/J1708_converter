#include "stm8s_conf.h"
#include "init.h"
#include "softuart.h"
#include "fifo.h"
//User defines
#define enIRQ asm("rim")
#define disIRQ asm("sim")
//Function definition
static void SysInit(void);
//Main section
uint8_t RxBuf;
enum MAIN_FSM{
  wait_mid, 
  wait_size, 
  wait_data, 
  wait_crc
};
static enum MAIN_FSM main_fsm = wait_mid;
static struct FIFO_STR swUART;
void main(void)
{
	SysInit();
        swUART.isEmpty = TRUE;
        jTransmitStr.MID = 0x55U;
        for(uint8_t i = 0; i < 21; ++i){
            jTransmitStr.data[i] = i;
        }
        jTransmitStr.CRC = 0xFFU;
        jTransmitStr.length = 0x17U;
        j1708FIFO.isEmpty = TRUE;
	for(;;){
          if(!j1708FIFO.isEmpty){
            if(tState == free_bus){
              jReceiveStr = jReceive(&j1708FIFO);//Get parse recieved ring buffer
            }
          }
          if(test_status(receive_buffer_full) == receive_buffer_full){//Receive data from software UART
            uart_read(&RxBuf);
            Push(&swUART, RxBuf);
          }
          static uint8_t u8CountData = 0x00;
          if(!swUART.isEmpty){
            switch(main_fsm){
            case wait_mid:
              jTransmitStr.MID = Pull(&swUART);
              main_fsm = wait_size;
              break;
              
            case wait_size:
              jTransmitStr.length = Pull(&swUART);
              main_fsm = wait_data;
              break;
              
            case wait_data:
              if(u8CountData < jTransmitStr.length){
                jTransmitStr.data[u8CountData++] = Pull(&swUART);
              }
              else{
                main_fsm = wait_crc;
              }
              break;
              
            case wait_crc:
              jTransmitStr.CRC = Pull(&swUART);
              break;
              
            default:
              main_fsm = wait_mid;
              while(!swUART.isEmpty){//Clear data
                Pull(&swUART);
              }
              break;
            }
          }
          //TODO main FSM
          //TODO soft UART answering
//          if(tState == free_bus){
//            jTransmit(&jTransmitStr, 1);
//          }
          //while(!jTransmit(&jTransmitStr));
          //for(uint16_t i = 0; i < 0xFFFF; ++i){asm("nop");}
          /*if(test_status(receive_buffer_full) == receive_buffer_full){
            uart_read(&RxBuf);
            UART1->DR = RxBuf;
          }  */
        }
}
//Function declaration
static void SysInit(void){
  CLK_Config();
  GPIO_Config();
  UART_Config();
  Tim1_Config();
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
#endif