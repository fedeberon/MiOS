#include <x86/io.h>
#include <x86/pic.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

/**
 * Remaps PIC IRQs from legacy vectors (0x08-0x0F) to 0x20-0x2F.
 */
void pic_remap(void) {
    uint8_t pic1_mask = inb(PIC1_DATA);
    uint8_t pic2_mask = inb(PIC2_DATA);

    outb(PIC1_COMMAND, 0x11);
    io_wait();
    outb(PIC2_COMMAND, 0x11);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, 0x01);
    io_wait();
    outb(PIC2_DATA, 0x01);
    io_wait();

    outb(PIC1_DATA, pic1_mask);
    outb(PIC2_DATA, pic2_mask);
}

/**
 * Sends End-Of-Interrupt signal for the handled IRQ line.
 */
void pic_send_eoi(uint8_t irq_line) {
    if (irq_line >= 8U) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}
