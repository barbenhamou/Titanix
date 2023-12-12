#include "../../include/lib/pit.h"

uint64_t current_tick = 0;

static void timer_func(isr_frame_t* frame) {
    current_tick++;
    monitor_write("TICK: ");
    monitor_put_dec(current_tick);
    monitor_write("\n");

}

void init_timer(uint32_t frequency) {
    idt_install_irq(32, (void*)&timer_func);

    uint32_t devisor = 1193180 / frequency;

    outb(0x43, 0x34);

    uint8_t l = (uint8_t)(devisor & 0xff);
    uint8_t h = (uint8_t)((devisor >> 8) & 0xff);

    outb(0x40, l);
    outb(0x40, h);

    __asm__ __volatile__("int $0x20");
}
