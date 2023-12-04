#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "functions.h"

typedef struct idt_gate_t {
    uint16_t isr_low;
    uint16_t segment_selector;
    uint8_t ist;
    uint8_t atrributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_gate_t;

typedef struct idtr_t {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

#define IDT_MAX_DISCRIPTORS 256
#define IDT_MAX_CPU_EXCEPTIONS 32

extern void exception_handler();

void idt_set_discriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist);

void idt_init();

uint8_t idt_allocate_vector();

void idt_free_vector(uint8_t vector);

void print_idt();

// typedef struct registers_t {
//     uint64_t ds;
//     uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;

// }

#endif