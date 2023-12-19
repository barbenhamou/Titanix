#include "../../include/lib/pic.h"

void pic_remap() {
    uint8_t master_mask, slave_mask;

    master_mask = inb(PIC_MASTER_DATA_PORT);
    slave_mask = inb(PIC_SLAVE_DATA_PORT);

    outb(PIC_MASTER_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    outb(PIC_SLAVE_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);

    outb(PIC_MASTER_DATA_PORT, PIC_OFFSET);
    outb(PIC_SLAVE_DATA_PORT, PIC_OFFSET + 0x08);

    outb(PIC_MASTER_DATA_PORT, PIC_ICW1_INTERVAL4);
    outb(PIC_SLAVE_DATA_PORT, PIC_ICW1_SINGLE);

    outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086);
    outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);

    outb(PIC_MASTER_DATA_PORT, master_mask);
    outb(PIC_SLAVE_DATA_PORT, slave_mask);
}

void pic_mask_irq(uint8_t irq_vector) {
    uint16_t port;
    uint8_t masks;

    if (irq_vector < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_vector -= 8;
    }

    masks = inb(port);
    masks |= (1 << irq_vector);
    outb(port, masks);
}

void pic_unmask_irq(uint8_t irq_vector) {
    uint16_t port;
    uint8_t masks;

    if (irq_vector < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_vector -= 8;
    }

    masks = inb(port);
    masks &= ~(1 << irq_vector);
    outb(port, masks);
}

void pic_send_eoi(uint8_t irq_vector) {
    if (irq_vector >= 40) {
        outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
    }
    outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void pic_disable() {
    pic_remap();
    for(uint8_t i = 0; i < 16; ++i) {
        pic_mask_irq(i);
    }
}