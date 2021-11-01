#include "stm8s.h"
#include "init.h"
//Function definitions
static void SysInit();
//Main section
int main()
{     
  SysInit();
  for(;;){
    GPIOD->ODR^=(1<<4);
  }
}
//Function declaration
//This function config all of periph MCU
static void SysInit(){
  CLK_Init();
  GPIO_Init();
  uart_init();
}
//Assert params for SPL library
#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  return;
}
#endif