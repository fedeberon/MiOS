#include <stdint.h>
#include <kernel/console.h>
#include <kernel/keyboard.h>
#include <x86/io.h>

#define INPUT_BUFFER_SIZE 128U

static char g_line_buffer[INPUT_BUFFER_SIZE];
static uint32_t g_line_length = 0;
static volatile int g_line_ready = 0;

/**
 * Minimal PS/2 set 1 scancode map to lowercase ASCII.
 */
static char scancode_to_ascii(uint8_t scancode) {
    static const char map[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
    };

    if (scancode < 128U) {
        return map[scancode];
    }
    return 0;
}

/**
 * Handles keyboard IRQ1 by consuming one scancode from port 0x60.
 */
void keyboard_handle_irq(void) {
    uint8_t scancode = inb(0x60);

    if ((scancode & 0x80U) != 0U) {
        return;
    }

    if (g_line_ready != 0) {
        return;
    }

    char c = scancode_to_ascii(scancode);
    if (c == 0) {
        return;
    }

    if (c == '\n') {
        g_line_buffer[g_line_length] = '\0';
        g_line_ready = 1;
        console_putc('\n');
        return;
    }

    if (c == '\b') {
        if (g_line_length > 0U) {
            g_line_length--;
            g_line_buffer[g_line_length] = '\0';
            console_putc('\b');
        }
        return;
    }

    if (g_line_length < (INPUT_BUFFER_SIZE - 1U)) {
        g_line_buffer[g_line_length++] = c;
        g_line_buffer[g_line_length] = '\0';
        console_putc(c);
    }
}

/**
 * Returns non-zero when Enter completed a command line.
 */
int keyboard_line_available(void) {
    return g_line_ready;
}

/**
 * Returns the current completed command line.
 */
const char* keyboard_line_buffer(void) {
    return g_line_buffer;
}

/**
 * Releases line buffer for next user command.
 */
void keyboard_consume_line(void) {
    g_line_length = 0;
    g_line_buffer[0] = '\0';
    g_line_ready = 0;
}
