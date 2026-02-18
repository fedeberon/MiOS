#include <stdint.h>
#include <x86/io.h>
#include <x86/pit.h>

#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND       0x43
#define PIT_BASE_FREQ     1193182U

static volatile uint32_t g_ticks = 0;

/**
 * Programs PIT channel 0 to generate periodic interrupts.
 */
void pit_init(uint32_t frequency_hz) {
    if (frequency_hz == 0U) {
        frequency_hz = 100U;
    }

    uint32_t divisor = PIT_BASE_FREQ / frequency_hz;
    if (divisor == 0U) {
        divisor = 1U;
    }

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0_DATA, (uint8_t)(divisor & 0xFFU));
    outb(PIT_CHANNEL0_DATA, (uint8_t)((divisor >> 8) & 0xFFU));
}

/**
 * Returns the current timer tick counter.
 */
uint32_t pit_ticks(void) {
    return g_ticks;
}

/**
 * Increments the global tick counter on every PIT IRQ.
 */
void pit_on_tick(void) {
    g_ticks++;
}
