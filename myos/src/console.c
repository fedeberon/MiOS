#include <kernel/console.h>

static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const uint8_t VGA_WIDTH = 80;
static const uint8_t VGA_HEIGHT = 25;
static const uint8_t VGA_COLOR = 0x07;

static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

/**
 * Clears full VGA text screen and resets cursor position.
 */
void console_clear(void) {
    for (uint16_t i = 0; i < (uint16_t)(VGA_WIDTH * VGA_HEIGHT); i++) {
        VGA_BUFFER[i] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)' ';
    }
    cursor_row = 0;
    cursor_col = 0;
}

/**
 * Writes one character at current cursor and advances it.
 */
void console_putc(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else {
        VGA_BUFFER[cursor_row * VGA_WIDTH + cursor_col] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)c;
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
 * Writes a null-terminated ASCII string to VGA console.
 */
void console_write(const char* str) {
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        console_putc(str[i]);
    }
}

/**
 * Writes an unsigned 32-bit integer in decimal.
 */
void console_write_u32(uint32_t value) {
    char buffer[11];
    uint8_t index = 0;

    if (value == 0U) {
        console_putc('0');
        return;
    }

    while (value > 0U && index < sizeof(buffer)) {
        buffer[index++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (index > 0U) {
        index--;
        console_putc(buffer[index]);
    }
}
