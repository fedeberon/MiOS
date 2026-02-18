#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

/**
 * Handles one keyboard IRQ event by reading scancode and updating input state.
 */
void keyboard_handle_irq(void);

/**
 * Returns non-zero when a full input line is available.
 */
int keyboard_line_available(void);

/**
 * Returns pointer to the current completed input line.
 */
const char* keyboard_line_buffer(void);

/**
 * Marks current completed line as consumed so user can type next command.
 */
void keyboard_consume_line(void);

#endif
