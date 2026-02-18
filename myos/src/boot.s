; Multiboot v1 header so GRUB can load this kernel.
section .multiboot
align 4
    dd 0x1BADB002
    dd 0x0
    dd -(0x1BADB002 + 0x0)

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
bits 32

global _start
extern kernel_main

; First instruction executed after GRUB transfers control.
_start:
    cli
    mov esp, stack_top
    call kernel_main

.hang:
    hlt
    jmp .hang
