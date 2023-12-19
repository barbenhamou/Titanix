#pragma once

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "functions.h"
#include "monitor.h"
#include "pic.h"

#define PAGE_FAULT 0x0e
#define DOUBLE_FAULT 0x08
#define DIVISION_EXCEPTION 0x00
#define GP_FAULT 0x0d

#define IDT_MAX_DISCRIPTORS 256
#define IDT_MAX_CPU_EXCEPTIONS 32



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

typedef struct isr_frame_t {
    struct {
        uint64_t cr4;
        uint64_t cr3;
        uint64_t cr2;
        uint64_t cr0;
    } __attribute__((packed)) control_registers;

    struct {
        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t r11;
        uint64_t r10;
        uint64_t r9;
        uint64_t r8;
        uint64_t rdi;
        uint64_t rsi;
        uint64_t rdx;
        uint64_t rcx;
        uint64_t rbx;
        uint64_t rax;
    } __attribute__((packed)) general_purpose_registers;

    struct {
        uint64_t rbp;
        uint64_t vector;
        uint64_t error_code;
        uint64_t rip;
        uint64_t segment_selector;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t dss;
    } __attribute__((packed)) basic_frame;
    
} __attribute__((packed)) isr_frame_t;

extern uint64_t routine_services[IDT_MAX_DISCRIPTORS];

extern void exception_handler(isr_frame_t* frame);

extern void irq_handler(isr_frame_t* irq);

void analyze_page_fault(isr_frame_t* frame);

void analyze_double_fault(isr_frame_t* frame);

void analyze_general_protection(isr_frame_t* frame);

void analyze_division_by_zero(isr_frame_t* frame);

void analyze_software_interrupt(isr_frame_t* frame);

void idt_set_discriptor(uint8_t vector, uint64_t isr, uint8_t flags, uint8_t ist);

void idt_init();

void idt_install_irq(uint8_t vector, void *handler);

uint8_t idt_allocate_vector();

void idt_free_vector(uint8_t vector);

void print_idt();

#endif