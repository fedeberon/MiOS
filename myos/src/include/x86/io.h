#ifndef X86_IO_H
#define X86_IO_H

#include <stdint.h>

/**
 * Reads one byte from an I/O port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

/**
 * Writes one byte to an I/O port.
 */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * Small delay for legacy devices and PIC remap sequencing.
 */
static inline void io_wait(void) {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

#endif
