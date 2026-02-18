#include <stdint.h>
#include <kernel/console.h>
#include <kernel/shell.h>
#include <x86/gdt.h>
#include <x86/idt.h>
#include <x86/pit.h>

/**
 * Kernel entry point called from boot assembly.
 *
 * Brings up low-level CPU tables (GDT/IDT), configures PIC/PIT,
 * enables interrupts, and enters the interactive shell loop.
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
    console_write("System ready.\n\n");
    shell_init();

    for (;;) {
        shell_poll();
        __asm__ volatile ("hlt");
    }
}
