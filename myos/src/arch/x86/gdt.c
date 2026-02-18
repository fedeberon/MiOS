#include <stdint.h>
#include <x86/gdt.h>

/**
 * One 8-byte GDT entry as defined by x86 protected mode.
 */
typedef struct __attribute__((packed)) {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} gdt_entry_t;

/**
 * GDTR register layout used by lgdt.
 */
typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} gdt_ptr_t;

static gdt_entry_t gdt_entries[3];
static gdt_ptr_t gdt_ptr;

extern void gdt_flush(uint32_t gdt_ptr_addr);

/**
 * Fills one descriptor in the GDT.
 */
static void gdt_set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[index].base_low = (uint16_t)(base & 0xFFFFU);
    gdt_entries[index].base_middle = (uint8_t)((base >> 16) & 0xFFU);
    gdt_entries[index].base_high = (uint8_t)((base >> 24) & 0xFFU);

    gdt_entries[index].limit_low = (uint16_t)(limit & 0xFFFFU);
    gdt_entries[index].granularity = (uint8_t)((limit >> 16) & 0x0FU);
    gdt_entries[index].granularity |= (uint8_t)(granularity & 0xF0U);
    gdt_entries[index].access = access;
}

/**
 * Initializes a flat GDT with null, kernel code, and kernel data segments.
 */
void gdt_init(void) {
    gdt_ptr.limit = (uint16_t)(sizeof(gdt_entry_t) * 3U - 1U);
    gdt_ptr.base = (uint32_t)(uintptr_t)&gdt_entries;

    gdt_set_gate(0, 0U, 0U, 0U, 0U);
    gdt_set_gate(1, 0U, 0xFFFFFU, 0x9AU, 0xCFU);
    gdt_set_gate(2, 0U, 0xFFFFFU, 0x92U, 0xCFU);

    gdt_flush((uint32_t)(uintptr_t)&gdt_ptr);
}
