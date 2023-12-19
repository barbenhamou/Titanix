#ifndef PIC_H
#define PIC_H

#include "types.h"
#include "functions.h"

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_COMMAND_PORT 0xa0
#define PIC_SLAVE_DATA_PORT 0xa1

#define PIC_EOI 0x20
#define PIC_OFFSET 0x20

#define PIC_ICW1_ICW4	    0x01
#define PIC_ICW1_SINGLE	    0x02
#define PIC_ICW1_INTERVAL4  0x04
#define PIC_ICW1_LEVEL	    0x08
#define PIC_ICW1_INIT	    0x10

#define PIC_ICW4_8086	    0x01
#define PIC_ICW4_AUTO	    0x02
#define PIC_ICW4_BUF_SLAVE  0x08
#define PIC_ICW4_BUF_MASTER 0x0C

void pic_remap();

void pic_mask_irq(uint8_t irq_vector);

void pic_unmask_irq(uint8_t irq_vector);

void pic_send_eoi(uint8_t irq_vector);

void pic_disable();

#endif