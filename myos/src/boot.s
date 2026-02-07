; Header Multiboot v1 para que GRUB reconozca el kernel.
SECTION .multiboot
align 4
    dd 0x1BADB002
    dd 0x0
    dd -(0x1BADB002 + 0x0)

; Código de arranque en 32 bits.
SECTION .text
bits 32

global _start
extern kernel_main

; Punto de entrada: deshabilita interrupciones, llama al kernel y se queda en halt.
_start:
    cli
    call kernel_main

.hang:
    hlt
    jmp .hang
