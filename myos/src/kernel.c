#include <stdint.h>

/* Buffer de texto VGA (modo 80x25) mapeado en memoria. */
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
/* Dimensiones del modo texto. */
static const uint8_t VGA_WIDTH = 80;
static const uint8_t VGA_HEIGHT = 25;
/* Atributo de color: gris claro sobre negro. */
static const uint8_t VGA_COLOR = 0x07;

/* Cursor de escritura actual. */
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

/**
 * Lee un byte de un puerto de E/S.
 *
 * @param port Puerto de E/S.
 * @return Valor leído.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * Escribe un caracter en la posición actual del cursor.
 *
 * Maneja salto de línea y avance del cursor. Si llega al final de la pantalla,
 * vuelve al inicio (sin scroll).
 *
 * @param c Caracter ASCII a imprimir.
 */
static void vga_putc(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            VGA_BUFFER[cursor_row * VGA_WIDTH + cursor_col] =
                ((uint16_t)VGA_COLOR << 8) | (uint8_t)' ';
        }
        return;
    } else {
        VGA_BUFFER[cursor_row * VGA_WIDTH + cursor_col] =
            ((uint16_t)VGA_COLOR << 8) | (uint8_t)c;
        cursor_col++;
    }

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }
    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0;
    }
}

/**
 * Writes a null-terminated string to the VGA text buffer.
 *
 * This function assumes the VGA text mode buffer starts at 0xB8000 and
 * uses a fixed color attribute. It relies on the global cursor.
 *
 * @param str Pointer to a null-terminated ASCII string.
 */
static void vga_write(const char* str) {
    uint16_t index = 0;
    while (str[index] != '\0') {
        vga_putc(str[index]);
        index++;
    }
}

/**
 * Convierte scancodes PS/2 a ASCII básico (sin mayúsculas ni teclas especiales).
 *
 * @param sc Scancode de teclado.
 * @return Caracter ASCII o 0 si no se mapea.
 */
static char scancode_to_ascii(uint8_t sc) {
    static const char map[128] = {
        0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
        'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
        'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
    };
    if (sc < 128) {
        return map[sc];
    }
    return 0;
}

/**
 * Bucle de polling del teclado PS/2.
 *
 * Lee el puerto 0x64 para saber si hay datos y luego lee el scancode de 0x60.
 * Imprime caracteres ASCII básicos en pantalla.
 */
static void keyboard_poll(void) {
    for (;;) {
        if (inb(0x64) & 0x01) {
            uint8_t sc = inb(0x60);
            if (sc & 0x80) {
                continue;
            }
            char c = scancode_to_ascii(sc);
            if (c) {
                vga_putc(c);
            }
        }
    }
}

/**
 * Kernel entry point called from the boot assembly.
 *
 * Prints two lines to the VGA text buffer and then enters keyboard polling.
 */
void kernel_main(void) {
    vga_write("Hello from MyOS kernel!\n");
    vga_write("If you can read this, GRUB -> kernel works.\n");

    keyboard_poll();
}
