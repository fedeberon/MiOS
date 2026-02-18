# MyOS

MyOS es un kernel x86 (32 bits) minimo que bootea con GRUB (Multiboot v1),
usa consola VGA en modo texto, interrupciones basicas (GDT/IDT/PIC/PIT)
y un shell interactivo con comandos internos.

## Construccion

1. Construir imagen base de toolchain (si no esta publicada localmente):
   `docker build -f Dockerfile.toolchain -t myos-toolchain .`

2. Construir imagen runtime:
   `docker build -t myos-build .`

3. Generar ISO:
   `docker run --rm -v "$PWD":/work -w /work myos-build make clean all`

Salida esperada: `dist/myos.iso`.

## Prueba rapida en emulador

Con QEMU (x86):

`qemu-system-i386 -cdrom dist/myos.iso -boot d -m 64`

## Comandos de shell disponibles

Cuando el kernel arranca, muestra prompt `myos>`.

- `help`: lista comandos disponibles.
- `clear`: limpia la pantalla.
- `echo <texto>`: imprime texto.
- `ticks`: muestra ticks del timer PIT.
- `reboot`: solicita reinicio via controlador de teclado.

## Estructura de archivos

- `Dockerfile.toolchain`: compila e instala cross-compiler `i686-elf` y utilidades.
- `Dockerfile`: imagen liviana que reutiliza el toolchain.
- `Makefile`: reglas de build, link y empaquetado de ISO.
- `linker.ld`: layout de secciones y direccion de carga (1 MiB).
- `grub.cfg`: entrada de boot de GRUB.
- `src/boot.s`: entrada inicial de kernel y setup de stack.
- `src/kernel.c`: secuencia de inicializacion del kernel.
- `src/console.c`: salida de texto por VGA.
- `src/keyboard.c`: entrada de teclado por IRQ1 y buffer de linea.
- `src/shell.c`: parser y ejecucion de comandos del shell.
- `src/arch/x86/gdt.c`: Global Descriptor Table.
- `src/arch/x86/idt.c`: Interrupt Descriptor Table + handler central.
- `src/arch/x86/interrupts.s`: stubs ASM de ISR/IRQ y flush de GDT.
- `src/arch/x86/pic.c`: controlador PIC 8259.
- `src/arch/x86/pit.c`: timer PIT y contador de ticks.
- `src/include/`: interfaces publicas de modulos.
- `docs/ARCHITECTURE.md`: explicacion detallada de arquitectura y flujo.
