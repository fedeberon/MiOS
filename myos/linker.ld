/* Punto de entrada del kernel. */
ENTRY(_start)

SECTIONS
{
  /* Cargar el kernel a partir de 1 MiB. */
  . = 1M;

  /* Header Multiboot requerido por GRUB. */
  .multiboot :
  {
    *(.multiboot)
  }

  /* Código ejecutable. */
  .text :
  {
    *(.text*)
  }

  /* Datos de solo lectura (strings, constantes). */
  .rodata :
  {
    *(.rodata*)
  }

  /* Datos inicializados. */
  .data :
  {
    *(.data*)
  }

  /* Datos sin inicializar. */
  .bss :
  {
    *(COMMON)
    *(.bss*)
  }
}
