# MyOS

## Construcción (toolchain una sola vez)

1. Construir la imagen base del toolchain (solo la primera vez):
   docker build -f Dockerfile.toolchain -t myos-toolchain .

2. (Opcional) Publicar en Docker Hub:
   docker tag myos-toolchain fedeberon/mi-os:toolchain-0.1.0
   docker push fedeberon/mi-os:toolchain-0.1.0

## Construcción rápida del ISO

Usa el Dockerfile liviano que reusa el toolchain:

1. Construir la imagen liviana:
   docker build -t myos-build .

2. Construir el ISO:
   docker run --rm -v "$PWD":/work -w /work myos-build make clean all

Esto genera `dist/myos.iso`, un ISO booteable con GRUB Multiboot (v1) que imprime:
- "Hello from MyOS kernel!"
- "If you can read this, GRUB -> kernel works."
Luego entra en un loop de polling PS/2 y muestra teclas ASCII básicas en pantalla.

## Archivos del proyecto

- `Dockerfile.toolchain`: imagen pesada que compila el cross-compiler `i686-elf` y utilidades.
- `Dockerfile`: imagen liviana que reusa el toolchain ya compilado.
- `Makefile`: automatiza la compilación del kernel, el link y la creación del ISO con GRUB.
- `linker.ld`: script del linker que coloca el kernel a partir de 1 MiB y ordena secciones.
- `grub.cfg`: configuración mínima de GRUB para cargar el kernel.
- `src/boot.s`: stub de arranque con header Multiboot v1 y salto a `kernel_main`.
- `src/kernel.c`: código del kernel que escribe texto en VGA.
- `.gitignore`: ignora artefactos de build.
