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
        swUART.isEmpty = TRUE;//A little of black magic 
        j1708FIFO.isEmpty = TRUE;
	for(;;){
          if(!j1708FIFO.isEmpty){//Check for j1708 end of transaction
            if(tState == free_bus){
              jReceiveStr = jReceive(&j1708FIFO);//Get parse recieved ring buffer
            }
          }
          if(test_status(receive_buffer_full) == receive_buffer_full){//Receive data from software UART
            uart_read(&RxBuf);//Load data into buffer uart_sw
            Push(&swUART, RxBuf);//Load data into ring buffer from uart_sw
          }
          static uint8_t u8CountData = 0x00;
          if(!swUART.isEmpty){//Check UART buffer for new data
            switch(main_fsm){//Parse data from packet frames
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
              jTransmitStr.CRC = Pull(&swUART);//It's not workig CRC
              break;
              
            default:
              main_fsm = wait_mid;
              while(!swUART.isEmpty){//Clear data
                Pull(&swUART);
              }
              break;
            }
          }
        }
}
//Function declaration
static void SysInit(void){
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
#endif