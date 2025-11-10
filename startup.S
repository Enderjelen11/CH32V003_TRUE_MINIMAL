.section .text.startup
    .globl _start
    .globl _estack
    .globl main

_start:
    la   sp, _estack
    call main
1:  j    1b

    .section .vector_table, "a", @progbits
    .align 4
    .globl __vectors
    .globl EXTI7_0_IRQHandler
    .globl Default_Handler

__vectors:
    .word _start

    .rept 19
        .word Default_Handler
    .endr

    .word EXTI7_0_IRQHandler

    .rept (32 - 21)
        .word Default_Handler
    .endr

    .section .text.Default_Handler, "ax", @progbits
Default_Handler:
    j Default_Handler
