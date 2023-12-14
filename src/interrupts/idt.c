#include "../../include/lib/interrupts.h"

extern void load_idt(idtr_t *idtr);
extern uint64_t isr_stub_table[];

idt_gate_t idt[IDT_MAX_DISCRIPTORS];
bool_t vectors[IDT_MAX_DISCRIPTORS];
uint64_t routine_services[IDT_MAX_DISCRIPTORS];
idtr_t idtr;

void idt_set_discriptor(uint8_t vector, uint64_t isr, uint8_t flags, uint8_t ist) {
    idt_gate_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xffff;
    descriptor->segment_selector = 0x0008; //gdt.code64
    descriptor->ist = ist;
    descriptor->atrributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xffff;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xffffffff;
    descriptor->reserved = 0;
}

void idt_init() {
    idtr.base = (uint64_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_gate_t) * IDT_MAX_DISCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; ++vector) {
        idt_set_discriptor(vector, isr_stub_table[vector], 0x8e, 0);
        vectors[vector] = TRUE;
    }

    outb(PIC_MASTER_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    outb(PIC_SLAVE_COMMAND_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);

    outb(PIC_MASTER_DATA_PORT, PIC_EOI);
    outb(PIC_SLAVE_DATA_PORT, PIC_EOI + 0x08);

    outb(PIC_MASTER_DATA_PORT, PIC_ICW1_INTERVAL4);
    outb(PIC_SLAVE_DATA_PORT, PIC_ICW1_SINGLE);

    outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086);
    outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);
    load_idt(&idtr);
    __asm__ __volatile__("sti");
}

void idt_install_irq(uint8_t vector, void *handler) {
    routine_services[vector] = (uint64_t)handler;
    idt_set_discriptor(vector, isr_stub_table[vector], 0x8e, 0);
}

uint8_t idt_allocate_vector() {
    for (uint32_t i = 0; i < IDT_MAX_DISCRIPTORS; ++i) {
        if (!vectors[i]) {
            vectors[i] = TRUE;
            return (uint8_t)i;
        }
    }

    return 0;
}

void idt_free_vector(uint8_t vector) {
    idt_set_discriptor(vector, 0, 0, 0);
    routine_services[vector] = 0;
    vectors[vector] = 0;
}

void print_idt() {
    INFO("_________________IDT_________________\n\t");
    for (uint32_t i = 0; i < IDT_MAX_DISCRIPTORS; ++i) {
        if (vectors[i]) {
            idt_gate_t* gate = &idt[i];
            uint64_t isr = ((uint64_t)gate->isr_low) | ((uint64_t)gate->isr_mid << 16) | ((uint64_t)gate->isr_high << 32);
            uint8_t flags = gate->atrributes;
            uint8_t ist = gate->ist;
            uint16_t segment = gate->segment_selector;

            INFO("id: %d\n", i);
            INFO("\t isr: %x\n", i < 32? isr : routine_services[i]);
            INFO("\t flags %x\n", flags);
            INFO("\t ist: %d\n", ist);
            INFO("\t segment selector: %x", segment);
        }
    }
    INFO("_____________________________________\n");
}
