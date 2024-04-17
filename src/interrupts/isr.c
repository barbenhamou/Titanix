#include "../../include/lib/interrupts.h"
#include "../../include/lib/monitor.h"

static const char_t* exception_labels[] = {
    "[0x00] Divide by Zero Exception",
    "[0x01] Debug Exception",
    "[0x02] Unhandled Non-maskable Interrupt",
    "[0x03] Breakpoint Exception",
    "[0x04] Overflow Exception",
    "[0x05] Bound Range Exceeded Exception",
    "[0x06] Invalid Opcode/Operand Exception",
    "[0x07] Device Unavailable Exception",
    "[0x08] Double Fault",
    "[0x09] Coprocessor Segment Overrun",
    "[0x0A] Invalid TSS Exception",
    "[0x0B] Absent Segment Exception",
    "[0x0C] Stack-segment Fault",
    "[0x0D] General Protection Fault",
    "[0x0E] Page Fault ",
    "[0x0F] Inexplicable Error",
    "[0x10] x87 Floating Exception",
    "[0x11] Alignment Check",
    "[0x12] Machine Check",
    "[0x13] SIMD Floating Exception",
    "[0x14] Virtualized Exception",
    "[0x15] Control Protection Exception",
    "[0x16] Inexplicable Error",
    "[0x17] Inexplicable Error",
    "[0x18] Inexplicable Error",
    "[0x19] Inexplicable Error",
    "[0x1A] Inexplicable Error",
    "[0x1B] Inexplicable Error",
    "[0x1C] Hypervisor Intrusion Exception",
    "[0x1D] VMM Communications Exception",
    "[0x1E] Security Exception",
    "[0x1F] Inexplicable Error",
    "[0x20] For Now Thing"
};


void exception_handler(isr_frame_t* frame) {
    if (frame->basic_frame.vector == PAGE_FAULT) {
        analyze_page_fault(frame);
    } else if (frame->basic_frame.vector == DOUBLE_FAULT) {
        analyze_double_fault(frame);
    } else if (frame->basic_frame.vector == GP_FAULT) {
        analyze_general_protection(frame);
    } else if (frame->basic_frame.vector == DIVISION_EXCEPTION) {
        analyze_division_by_zero(frame);
    } else {
        analyze_software_interrupt(frame);
    }
}

void analyze_page_fault(isr_frame_t* frame) {
    monitor_write(exception_labels[frame->basic_frame.vector]);
    uint64_t faulting_address;
    __asm__ __volatile__("mov %%cr2, %0": "=r" (faulting_address));
    monitor_put_hex(faulting_address);
    monitor_put('\n');
    uint8_t err = frame->basic_frame.error_code;
    uint8_t present_bit = err & 0b00000001;
    uint8_t write_bit = (err & 0b00000010) >> 1;
    uint8_t reserved_bit = (err & 0b00000100) >> 2;

    monitor_write("\npresent-bit = ");
    monitor_put_dec(present_bit);
    monitor_write("\nwrite-bit = ");
    monitor_put_dec(write_bit);
    monitor_write("\nreserved-bit = ");
    monitor_put_dec(reserved_bit);

    ERROR("%x\n", faulting_address);
    __asm__ __volatile__ ("cli;hlt");
}

void analyze_double_fault(isr_frame_t* frame) {
    monitor_write(exception_labels[frame->basic_frame.vector]);
    monitor_put('\n');
    __asm__ __volatile__ ("cli;hlt");
}

void analyze_general_protection(isr_frame_t* frame) {
    monitor_write(exception_labels[frame->basic_frame.vector]);
    monitor_put('\n');
    __asm__ __volatile__ ("cli;hlt");
}

void analyze_division_by_zero(isr_frame_t* frame) {
    monitor_write(exception_labels[frame->basic_frame.vector]);
    monitor_put('\n');
    __asm__ __volatile__ ("cli;hlt");
}

void analyze_software_interrupt(isr_frame_t* frame) {
    monitor_write(exception_labels[frame->basic_frame.vector]);
    monitor_put('\n');
    __asm__ __volatile__ ("cli;hlt");
}

void irq_handler(isr_frame_t* irq) {
    if (routine_services[irq->basic_frame.vector] != 0) {
        void *handler = (void*)routine_services[irq->basic_frame.vector];
        void (*handler_func)(isr_frame_t* frame) = (void (*)(isr_frame_t* frame))handler;
        handler_func(irq);
    }
}
