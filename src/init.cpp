#include "init.h"
/*CLK_Init
@inval: none
@outval: none
@func: clk configure*/
void CLK_Init(){
  CLK->CKDIVR = 0x00;//prescaler 1
}
/*GPIO_Init
@inval: none
@outval: none
@func: gpio configure*/
void GPIO_Init(){
  GPIOB->DDR|=(1<<5);//LED LIFE
  GPIOD->DDR|=(1<<4);//LED RUN
  GPIOB->CR1|=(1<<5);
  GPIOD->CR1|=(1<<4);
}