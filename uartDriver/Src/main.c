#include "../../drivers/stm32f401xe.h"

// ahb1
#define GPIOAEN (1U<<0)
// gpioa
#define GREENLED (1U<<5)

// apb1
#define USART2EN (1U<<17)

#define sysclk 16e6 // peripheral clock (16 MHz)
#define baudrate 115200


void uart_init(void) {

  RCC->AHB1ENR |= GPIOAEN;

  // set PA2 to alternate funciton mode (10)
  GPIOA->MODER |= (1U<<5);
  GPIOA->MODER &= ~(1U<<4);

  // set alternate function register to AF07 (0111)
  GPIOA->AFR[0] &= ~(1U<<11);
  GPIOA->AFR[0] |= (1U<<10);
  GPIOA->AFR[0] |= (1U<<9);
  GPIOA->AFR[0] |= (1U<<8);

  // set PA3 to alternate funciton mode (10)
  GPIOA->MODER |= (1U<<7);
  GPIOA->MODER &= ~(1U<<6);

  // set alternate function register to AF07 (0111)
  GPIOA->AFR[0] &= ~(1U<<15);
  GPIOA->AFR[0] |= (1U<<14);
  GPIOA->AFR[0] |= (1U<<13);
  GPIOA->AFR[0] |= (1U<<12);

  RCC->APB1ENR |= USART2EN;
  // baud rate register (~139,38)
  USART2->BRR = ((sysclk + (baudrate / 2U)) / baudrate);
  // control register (transmitter enable, clear everything else)
  USART2->CR1 = (1U<<3);
  // uart enable
  USART2->CR1 |= (1U<<13);
}

// write
void tx(int ch) {
  // check status register if transmit data register is empy (if not, it waits)
  while (!(USART2->SR & (1U<<7))){}

  // wirte to data register
  USART2->DR = (ch & 0xFF);
}

// read
char rx(void) {
  // check status register if read data register is empy (if not, it waits)
  while (!(USART2->SR & (1U<<5))){}
  // return data register
  return USART2->DR;
}

char myinp;

int main(void) {

  uart_init();

  // led to output mode
  GPIOA->MODER |= (1U<<10);
  GPIOA->MODER &= ~(1U<<11);

  while (1) {
    tx('A');
    //myinp = rx();
    //if (myinp == '1') {
    //  GPIOA->ODR |= GREENLED;
    //} else {
    //  GPIOA->ODR &= ~GREENLED;
    //}
  }
}
