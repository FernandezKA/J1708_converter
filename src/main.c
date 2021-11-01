#include "stm8s_conf.h"
#include "init.h"
#include "softuart.h"
#define enIRQ asm("rim")
#define disIRQ asm("sim")
//Function definition
static void SysInit(void);
//Main section
void main(void)
{
	SysInit();
	for(;;){
        for(int j = 0; j < 0xFF; ++j){
    uart_send(j);
    for(uint16_t i = 0; i < 0xFFFF; ++i) {asm("nop");}
    }
        }
}
//Function declaration
static void SysInit(void){
  CLK_Config();
  GPIO_Config();
  uart_init();
  enIRQ;
}
//Assert failed for SPL 
#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif