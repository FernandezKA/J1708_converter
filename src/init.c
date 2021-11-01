#include "init.h"
/*CLK_Init
@inval: none
@outval: none
@func: clk configure*/
void CLK_Config(void){
  CLK->CKDIVR = 0x00;//prescaler 1
}
/*GPIO_Init
@inval: none
@outval: none
@func: gpio configure*/
void GPIO_Config(void){
  GPIOB->DDR|=(1<<5);//LED LIFE
  GPIOD->DDR|=(1<<4);//LED RUN
  GPIOB->CR1|=(1<<5);
  GPIOD->CR1|=(1<<4);
}
//This function init hardware uart
void UART_Config(void){
  CLK->PCKENR1 |= CLK_PCKENR1_UART1; //ENABLE CLOCKING
  UART1->BRR1 = 0x68;
  UART1->BRR2 = 0x02;
  UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN;
}