#include <stdint.h>
#include <kernel/console.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <x86/io.h>
#include <x86/pit.h>

/**
 * Compares two null-terminated strings for full equality.
 */
static int str_eq(const char* a, const char* b) {
    uint32_t i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

/**
 * Returns non-zero if str starts with prefix.
 */
static int str_starts_with(const char* str, const char* prefix) {
    uint32_t i = 0;
    while (prefix[i] != '\0') {
        if (str[i] != prefix[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

/**
 * Prints shell prompt used for interactive commands.
 */
static void shell_prompt(void) {
    console_write("myos> ");
}

/**
 * Executes one parsed command line.
 */
static void shell_execute(const char* line) {
    if (line[0] == '\0') {
        return;
    }

    if (str_eq(line, "help")) {
        console_write("Commands: help clear echo ticks reboot\n");
        return;
    }

    if (str_eq(line, "clear")) {
        console_clear();
        return;
    }

    if (str_eq(line, "ticks")) {
        console_write("ticks=");
        console_write_u32(pit_ticks());
        console_write("\n");
        return;
    }

    if (str_eq(line, "reboot")) {
        console_write("Rebooting...\n");
        outb(0x64, 0xFE);
        return;
    }

    if (str_starts_with(line, "echo ")) {
        console_write(line + 5);
        console_write("\n");
        return;
    }

    if (str_eq(line, "echo")) {
        console_write("\n");
        return;
    }

    console_write("Unknown command: ");
    console_write(line);
    console_write("\n");
}

/**
 * Initializes shell banner and first prompt.
 */
void shell_init(void) {
    console_write("Type 'help' to list commands.\n");
    shell_prompt();
}

/**
 * Polls keyboard line buffer and executes command when ready.
 */
void shell_poll(void) {
    if (keyboard_line_available() == 0) {
        return;
    }

    const char* line = keyboard_line_buffer();
    shell_execute(line);
    keyboard_consume_line();
    shell_prompt();
}
