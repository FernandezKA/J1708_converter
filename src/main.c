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
void main(void)
{
	SysInit();
        jTransmitStr.MID = 0x55U;
        for(uint8_t i = 0; i < 21; ++i){
            jTransmitStr.data[i] = i;
        }
        jTransmitStr.CRC = 0xFFU;
        jTransmitStr.length = 0x17U;
        j1708FIFO.isEmpty = TRUE;
	for(;;){
          if(tState == stop_package){//After timeout begin parse received data
            if(!j1708FIFO.isEmpty){//If new data recieve from UART1, parse it
              
              UART1->DR = Pull(&j1708FIFO);
            } 
          }
          //TODO main FSM
          //TODO soft UART answering
          if(tState == free_bus){
            jTransmit(&jTransmitStr, 1);
          }
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