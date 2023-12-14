#include "../../include/lib/pit.h"

uint64_t current_tick = 0;
static uint8_t pit_vector;

static void timer_func(isr_frame_t* frame) {
    current_tick++;
    monitor_write("TICK: ");
    DEBUG("%d\n", current_tick);
    monitor_put_dec(current_tick);
    monitor_write("\n");

    if (pit_vector >= 40) {
        outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
    }
    outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void init_timer(uint32_t frequency) {
    uint32_t devisor = 1193180 / frequency;
    pit_vector = idt_allocate_vector();

    __asm__ __volatile__("cli");

    outb(0x43, 0x34);

    uint8_t l = (uint8_t)(8 & 0xff);
    uint8_t h = (uint8_t)((8 >> 8) & 0xff);

    outb(0x40, l);
    outb(0x40, h);

    __asm__ __volatile__("sti");

    idt_install_irq(pit_vector, (void*)&timer_func);
}
