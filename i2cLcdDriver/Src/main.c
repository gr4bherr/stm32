#include "../../drivers/stm32f401xe.h"
// using:
// HD44780U lcd
// pcf8574 i2c 8bit i/o expander

#define LCDADDR 0x4E // defualt (including r/w bit)
#define GPIOBEN (1<<1)
#define I2C1EN (1<<21)
#define PIN8 8 // pb8
#define PIN9 9 // pb9

// **** TIMER ****
void tim5_init(void) {
	RCC->APB1ENR |= (1<<3);

	TIM5->PSC = 90-1;
	TIM5->ARR = 0xffff;

  // enable counter
	TIM5->CR1 |= (1<<0);
  // wait for update interrupt flag
	while (!(TIM5->SR & (1<<0)));
}
void delayms(uint16_t ms) {
	for (uint16_t i=0; i<ms; i++) {
    TIM5->CNT = 0;
    while (TIM5->CNT < 1000);
	}
}

// **** I2C ****
void i2c_init(void) {
  RCC->AHB1ENR = GPIOBEN;
  RCC->APB1ENR |= I2C1EN;

	GPIOB->MODER |= (2<<PIN8*2) | (2<<PIN9*2);
	GPIOB->OTYPER |= (1<<PIN8) | (1<<PIN9);
	GPIOB->PUPDR |= (1<<PIN8*2) | (1<<PIN9*2);
	GPIOB->AFR[1] |= (4<<0) | (4<<4);

  // under reset state
  I2C1->CR1 |= (1<<15);
  // not under reset state
  I2C1->CR1 &= ~(1<<15);

  // set frequency to 45 MHz
  I2C1->CR2 |= (45<<0);
  // set clock
  I2C1->CCR = 225<<0;
  // set rise time
  I2C1->TRISE = 46;
  // peripheral enable
  I2C1->CR1 |= (1<<0);
}

// i2c write supporting multiple bytes
void i2c_burst_write (uint8_t Address, uint8_t *Data, int size) {
  // ** start **
  // enable ack
	I2C1->CR1 |= (1<<10);
  // generate start condition
	I2C1->CR1 |= (1<<8);
  // wait until start condition generated
	while (!(I2C1->SR1 & (1<<0)));

  // ** address **
  // send address
	I2C1->DR = Address;
  // wait until address flag set
	while (!(I2C1->SR1 & (1<<1)));
  // clear address flag
	uint8_t temp = I2C1->SR1 | I2C1->SR2;
  (void)temp;

  // ** write **
  // loop through all bytes
  for (int i=0; i<size; i++) {
    // wait until data register empty (tx)
    while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
    // send data
    I2C1->DR = *Data++;
    // byte transfer finished
    while (!(I2C1->SR1 & (1<<2)));
  }

  // ** stop **
  // generate stop
	I2C1->CR1 |= (1<<9);
}

void lcd_cmd(char cmd) {
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	i2c_burst_write(LCDADDR,(uint8_t *) data_t, 4);
}

void lcd_string(char *str) {
	while (*str) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (*str&0xf0);
    data_l = ((*str++<<4)&0xf0);
    data_t[0] = data_u|0x0D;  //en=1, rs=0
    data_t[1] = data_u|0x09;  //en=0, rs=0
    data_t[2] = data_l|0x0D;  //en=1, rs=0
    data_t[3] = data_l|0x09;  //en=0, rs=0
    i2c_burst_write(LCDADDR,(uint8_t *) data_t, 4);
  }
}

void lcd_init(void) {
  // initializing by instruction (4 bit)
  // init sequence
	delayms(50);
	lcd_cmd(0x30);
	delayms(5);
	lcd_cmd(0x30);
	delayms(1);
	lcd_cmd(0x30);

  // display setup
  lcd_cmd(0x20); // 4bit mode
	lcd_cmd(0x28); // 4bit data mode, 2 lines, 15x7 characters
	lcd_cmd(0x08); // turn display
	lcd_cmd(0x01); // clear display, move cursor home
	lcd_cmd(0x06); // move cursor right
	lcd_cmd(0x0C); // turn display, blink cursor
}

// **** MAIN ****
int main () {
	tim5_init();
	i2c_init();
	lcd_init();
	lcd_string("gr4bherr");
	while(1) {}
}
