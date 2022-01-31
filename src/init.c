#include "init.h"
/*CLK_Init
@inval: none
@outval: none
@func: clk configure*/
void CLK_Config(void)
{
  CLK->CKDIVR = 0x00; //prescaler 1
}
/*GPIO_Init
@inval: none
@outval: none
@func: gpio configure*/
void GPIO_Config(void)
{
  GPIOB->DDR |= (1 << 5); //LED LIFE
  GPIOD->DDR |= (1 << 4); //LED RUN
  GPIOB->CR1 |= (1 << 5);
  GPIOD->CR1 |= (1 << 4);
}
//This function init hardware uart
void UART_Config(void)
{
  CLK->PCKENR1 |= CLK_PCKENR1_UART1; //ENABLE CLOCKING
  UART1->BRR1 = 0x68;
  UART1->BRR2 = 0x02;
  UART1->CR2 |= UART1_CR2_RIEN;
  UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN;
}
//This function configure Tim1 for time measuring for arbitrating J1708 bus
void Tim1_Config(void)
{
  CLK->PCKENR1 |= CLK_PCKENR1_TIM1; //ENABLE CLOCKING
  TIM1->PSCRH = (0x10 >> 8);        //set prescaler
  TIM1->PSCRL = 0x10 & 0xFF;
  TIM1->ARRH = (0x80 >> 8);
  TIM1->ARRL = (0x80 & 0xFF);
  TIM1->IER |= TIM1_IER_UIE;
  TIM1->CR1 |= TIM1_CR1_CEN;
}
//This function config Tim4 for indicate an activity
void Tim4_Config(void)
{
  TIM4->CR1 |= TIM4_CR1_ARPE | TIM4_CR1_CEN;
  TIM4->IER |= TIM4_IER_UIE;
  TIM4->PSCR = 0x10;
  TIM4->ARR |= 100U;
}