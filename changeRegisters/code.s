// define atributes
.syntax unified
.cpu cortex-m4
.fpu softvfp // suport for floating point operations
.thumb

// global memory locations
.global vtable
.global reset_handler

// vecotr table
.type vtable, %object
vtable:
    .word _estack
    .word reset_handler // determines what to run on reset / power on
.size vtable, .-vtable


// **** basic program ****

.type reset_handler, %function

reset_handler:
  // stack pointer to the end of the stack
  LDR  r0, =_estack
  MOV  sp, r0

  // dummy values
  LDR  r7, =0xDEADBEEF
  MOVS r0, #0
  main_loop:
    // add 1 to register 'r0'.
    ADDS r0, r0, #1
    // Loop back.
    B    main_loop

.size reset_handler, .-reset_handler