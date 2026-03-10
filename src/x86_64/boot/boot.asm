section .text
bits 64
global _start
extern kernel_main

_start:
    mov rsp, stack_top
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 32768
stack_top: