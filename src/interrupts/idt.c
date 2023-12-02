#include "../../include/lib/interrupts.h"

extern void load_idt(idtr_t idtr);
extern void *isr_stub_table[];

void idt_set_discriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_gate_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xffff;
    descriptor->segment_selector = 0x0008; //gdt.code64
    descriptor->ist = 0;
    descriptor->atrributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xffff;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xffffffff;
    descriptor->reserved = 0;
}

void idt_init() {
    idtr.base = (uint64_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_gate_t) * IDT_MAX_DISCRIPTORS;

    for (uint8_t vector = 0; vector < 32; ++vector) {
        idt_set_discriptor(vector, isr_stub_table[vector], 0x8e);
        vectors[vector] = TRUE;
    }

    load_idt(idtr);
}