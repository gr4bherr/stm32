
/* end of stack  */
/* 512KB = 0x80000 */
_estack = 0x20080000; 

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
  RAM  (rxw) : ORIGIN = 0x20000000, LENGTH = 96K
}
