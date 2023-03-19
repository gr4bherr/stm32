
// peripherals
// AHB1 (bus)
#define AHB1 (0x40020000)

// general purpose I/O
// port a
#define GPIOA (AHB1 + 0x0)
#define GPIOA_MODE_R (*(volatile unsigned int *)(GPIOA + 0x0)) // (direction register)
#define GPIOA_OD_R (*(volatile unsigned int *)(GPIOA + 0x14)) // (data register)
#define GPIOAEN (1<<0) // io port a clock enable

// reset and clock control
#define RCC (AHB1 + 0x3800)
#define RCC_AHB1EN_R (*(volatile unsigned int *)(RCC + 0x30)) // peripheral clock enable register

// user LED PA5 (port A, pin 5)
#define GREENLED (1<<5)


int main(void) {

  // enable bus to transport a clock
	RCC_AHB1EN_R |= GPIOAEN;

  // setting port mode register (to 01)
	GPIOA_MODE_R |= (1<<10);
	GPIOA_MODE_R &=~ (1<<11);

	while(1) {
    // set green led to on
		//GPIOA_OD_R |= PIN5;

    // set green led to blink
		GPIOA_OD_R ^= GREENLED;
		for(int i=0;i<1000000;i++){}
	}
}
