#include "stm8s_conf.h"
#include "init.h"
#include "softuart.h"
#define enIRQ asm("rim")
#define disIRQ asm("sim")
//Function definition
static void SysInit(void);
//Main section
uint8_t RxBuf;
void main(void)
{
	SysInit();
	for(;;){
          if(test_status(receive_buffer_full) == receive_buffer_full){
            uart_read(&RxBuf);
          }  
        }
}
//Function declaration
static void SysInit(void){
  CLK_Config();
  GPIO_Config();
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