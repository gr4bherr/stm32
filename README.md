using:<br>
stm32f401re board (arm cortex m4 32bit risc)<br>
HD44780U lcd<br>
pcf8574 i2c 8bit i/o expander<br>

bare metal<br>
register level programming<br>
### stm32cubeide
 - [ledBlink](ledBlink/Src/main.c): just basic led blink
 - [gpioDriver](gpioDriver/Src/main.c): turn led on button press
 - [uartDriver](uartDriver/Src/main.c): transmits and recieves characters
 - [i2cLcdDriver](i2cLcdDriver/Src/main.c): basic lcd driver (with i2c)
### no ide
 - [changeRegisters](changeRegisters/): change value of registers r0 and r7
### other
 - [ledBlinkGhidra](ledBlinkGhidra/Src/main.c): just looking at what the code is actually doing (in ghidra)
 
 ([drivers](drivers/): official drivers from STM32CubeF4 MCU Package)
