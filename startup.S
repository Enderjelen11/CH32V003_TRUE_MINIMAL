    .section .text.startup
    .globl _start
    .globl _estack
    .globl main

_start:
    la sp, _estack
    call main
1:  j 1b

    .section .vector_table, "a", @progbits
    .align 4
    .globl __vectors

__vectors:
    .word _start
    .rept 31
        .word Default_Handler
    .endr

    .section .text.Default_Handler, "ax", @progbits
    .globl Default_Handler
Default_Handler:
    j Default_Handler
