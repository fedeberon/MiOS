#ifndef X86_PIC_H
#define X86_PIC_H

#include <stdint.h>

void pic_remap(void);
void pic_send_eoi(uint8_t irq_line);

#endif
