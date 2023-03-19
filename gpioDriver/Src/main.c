#include "../../drivers/stm32f401xe.h"

#define GPIOAEN (1<<0)
#define GPIOCEN (1<<2)

#define PIN5 (1<<5)
#define PIN13 (1<<13)

int main(void) {

  RCC->AHB1ENR |= GPIOAEN;
  RCC->AHB1ENR |= GPIOCEN;

  // set gpioa to output mode (01)
  GPIOA->MODER |= (1<<10);
  GPIOA->MODER &= ~(1<<11);

  // set gpioc to input mode (00)
  GPIOC->MODER &= ~(1<<26);
  GPIOC->MODER &= ~(1<<27);

  while(1) {
    // if user btn pressed -> turn on led
    if (GPIOC->IDR & PIN13) {
      GPIOA->BSRR = (1<<21); // reset
    } else {
      GPIOA->BSRR = PIN5; // set
    }
  }
}
