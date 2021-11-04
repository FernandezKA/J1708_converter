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
	for(;;){
          if(tState == free_bus){
            jTransmit(&jTransmitStr, 1);
            //GPIOB->ODR|=(1<<5);
          }
          else{
            asm("nop");
            //GPIOB->ODR&=~(1<<5);
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