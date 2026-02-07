# Herramientas del cross-compiler.
CC = i686-elf-gcc
AS = nasm
LD = i686-elf-ld

# Flags: freestanding (sin libc), 32-bit, y linker script propio.
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32
LDFLAGS = -T linker.ld -nostdlib

# Directorios de salida.
BUILD_DIR = build
DIST_DIR = dist
ISO_DIR = $(BUILD_DIR)/isofiles

# Artefactos principales.
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
ISO = $(DIST_DIR)/myos.iso

.PHONY: all clean iso

all: iso

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DIST_DIR):
	mkdir -p $(DIST_DIR)

$(BOOT_OBJ): src/boot.s | $(BUILD_DIR)
	$(AS) -f elf32 src/boot.s -o $(BOOT_OBJ)

$(KERNEL_OBJ): src/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/kernel.c -o $(KERNEL_OBJ)

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(BOOT_OBJ) $(KERNEL_OBJ)

# Crea la estructura de ISO y genera dist/myos.iso con GRUB.
iso: $(KERNEL_BIN) grub.cfg | $(DIST_DIR)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR) >/dev/null 2>&1

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
