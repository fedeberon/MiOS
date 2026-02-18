# Arquitectura actual de MyOS

Este documento describe que hace cada componente del kernel en esta etapa.

## Flujo de arranque

1. GRUB carga `kernel.bin` usando Multiboot v1.
2. `src/boot.s` toma control en `_start`, configura un stack propio y llama `kernel_main`.
3. `src/kernel.c` inicializa GDT, IDT, PIC y PIT.
4. Se habilitan interrupciones (`sti`) y el kernel entra en bucle `hlt`.
5. El PIT (IRQ0) genera ticks periodicos, atendidos por `isr_handler`.

## Que resuelve cada modulo

- `src/boot.s`
  - Header Multiboot v1.
  - Stack de 16 KiB en `.bss`.
  - Punto de entrada `_start`.

- `src/console.c`
  - Consola VGA en modo texto (0xB8000).
  - API: limpiar pantalla, escribir texto, imprimir numeros.

- `src/arch/x86/gdt.c`
  - Crea una GDT plana minima:
    - Descriptor nulo.
    - Segmento codigo kernel (selector `0x08`).
    - Segmento datos kernel (selector `0x10`).

- `src/arch/x86/interrupts.s`
  - `gdt_flush`: carga GDTR y recarga segmentos.
  - Stubs `isr0..isr31` para excepciones CPU.
  - Stubs `irq0..irq15` para hardware IRQ tras remapeo PIC.
  - `isr_common_stub`: guarda registros, llama a C (`isr_handler`) y retorna con `iretd`.

- `src/arch/x86/idt.c`
  - Crea y carga IDT de 256 entradas.
  - Remapea PIC a vectores `0x20..0x2F`.
  - Desenmascara IRQ0 (timer) y IRQ1 (keyboard) en PIC maestro.
  - `isr_handler`:
    - Excepcion CPU: imprime vector/error y detiene.
    - IRQ0: incrementa ticks.
    - IRQ hardware: envia EOI al PIC.

- `src/arch/x86/pic.c`
  - Logica de remapeo del 8259 PIC.
  - Envio de End-Of-Interrupt.

- `src/arch/x86/pit.c`
  - Configuracion del 8253/8254 PIT.
  - Contador global de ticks.

- `src/include/*`
  - Headers de interfaz para separar responsabilidades por modulo.

## Estado funcional

- Boot estable por GRUB Multiboot v1.
- Consola VGA funcional.
- Excepciones CPU enrutadas.
- Timer por interrupciones funcional (ticks).

## Proximos pasos sugeridos

1. Driver de teclado por IRQ1 (sin polling).
2. Scroll en consola VGA.
3. Pagina de memoria (paging) + asignador fisico.
4. Scheduler cooperativo minimo.
