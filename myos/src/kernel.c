#include <stdint.h>
#include <kernel/console.h>
#include <x86/gdt.h>
#include <x86/idt.h>
#include <x86/pit.h>

/**
 * Kernel entry point called from boot assembly.
 *
 * Brings up low-level CPU tables (GDT/IDT), configures PIC/PIT,
 * enables interrupts, and then idles while timer IRQs keep ticking.
 */
void kernel_main(void) {
    console_clear();
    console_write("MyOS kernel init...\n");

    gdt_init();
    console_write("[OK] GDT loaded\n");

    idt_init();
    console_write("[OK] IDT loaded + PIC remapped\n");

    pit_init(100U);
    console_write("[OK] PIT configured at 100 Hz\n");

    __asm__ volatile ("sti");
    console_write("[OK] Interrupts enabled\n");
    console_write("System ready.\n");

    uint32_t last_tick = 0;
    for (;;) {
        uint32_t now = pit_ticks();
        if (now != last_tick && (now % 100U) == 0U) {
            console_write("tick=");
            console_write_u32(now);
            console_write("\n");
            last_tick = now;
        }
        __asm__ volatile ("hlt");
    }
}
