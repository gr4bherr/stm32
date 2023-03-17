#define STM32F401xE
#include "../../drivers/stm32f4xx.h"
 
#define GPIOAEN (1U<<0)
#define GPIOCEN (1U<<2)

#define PIN5 (1U<<5)
#define PIN13 (1U<<13)

int main(void) {

  RCC->AHB1ENR |= GPIOAEN;
  RCC->AHB1ENR |= GPIOCEN;

  // set gpioa to output mode (01)
  GPIOA->MODER |= (1U<<10);
  GPIOA->MODER &= ~(1U<<11);

  // set gpioc to input mode (00)
  GPIOC->MODER &= ~(1U<<26);
  GPIOC->MODER &= ~(1U<<27);

  while(1) {
    // if user btn pressed -> turn on led
    if (GPIOC->IDR & PIN13) { 
      GPIOA->BSRR = (1U<<21); // reset
    } else {
      GPIOA->BSRR = PIN5; // set
    }
  }
}
