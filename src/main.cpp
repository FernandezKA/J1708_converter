#include "stm8s.h"
static void CLK_INIT(void){
  CLK->CKDIVR = 0x00;
}
static void GPIO_INIT(void){
  GPIOD->DDR|=(1<<4);
  GPIOD->CR1|=(1<<4);
}
int main()
{     
  CLK_INIT();
  GPIO_INIT();
  for(;;){
    GPIOD->ODR^=(1<<4);
  }
}
