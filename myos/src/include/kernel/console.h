#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include <stdint.h>

void console_clear(void);
void console_write(const char* str);
void console_putc(char c);
void console_write_u32(uint32_t value);

#endif
