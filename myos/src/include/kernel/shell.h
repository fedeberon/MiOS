#ifndef KERNEL_SHELL_H
#define KERNEL_SHELL_H

/**
 * Initializes shell state and prints initial prompt.
 */
void shell_init(void);

/**
 * Polls input state and executes one command when a full line is available.
 */
void shell_poll(void);

#endif
