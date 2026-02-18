# Herramientas del cross-compiler.
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

# Flags: freestanding (sin libc), 32-bit, y linker script propio.
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -Isrc/include
LDFLAGS = -T linker.ld -nostdlib

# Directorios de salida.
BUILD_DIR = build
DIST_DIR = dist
ISO_DIR = $(BUILD_DIR)/isofiles

# Fuentes C y ASM del kernel.
C_SOURCES = \
	src/kernel.c \
	src/console.c \
	src/arch/x86/gdt.c \
	src/arch/x86/idt.c \
	src/arch/x86/pic.c \
	src/arch/x86/pit.c

ASM_SOURCES = \
	src/boot.s \
	src/arch/x86/interrupts.s

# Objetos generados en build/ preservando estructura de carpetas.
OBJS = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) \
       $(patsubst src/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

KERNEL_BIN = $(BUILD_DIR)/kernel.bin
ISO = $(DIST_DIR)/myos.iso

.PHONY: all clean iso

all: iso

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

$(BUILD_DIR)/%.o: src/%.s | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJS)

# Crea la estructura de ISO y genera dist/myos.iso con GRUB.
iso: $(KERNEL_BIN) grub.cfg | $(DIST_DIR)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR) >/dev/null 2>&1

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
