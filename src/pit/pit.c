#include "../../include/lib/pit.h"

uint64_t current_tick = 0;
static uint8_t pit_vector;
static bool_t watching = FALSE, masked = FALSE, activated = FALSE;

static void timer_func(isr_frame_t* frame) {
    if (watching) {
        current_tick++;
        monitor_write("TICK: ");
        monitor_put_dec(current_tick);
        monitor_write("\n");
    }

    pic_send_eoi(pit_vector);
}

void init_timer(uint32_t frequency) {
    uint32_t devisor = 1193180 / frequency;
    pit_vector = idt_allocate_vector();

    __asm__ __volatile__("cli");

    outb(0x43, 0x34);

    uint8_t l = (uint8_t)(devisor & 0xff);
    uint8_t h = (uint8_t)((devisor >> 8) & 0xff);

    outb(0x40, l);
    outb(0x40, h);

    __asm__ __volatile__("sti");

    idt_install_irq(pit_vector, (void*)&timer_func);
    if (masked) {
        pic_unmask_irq(pit_vector);
    }
}

void pit_disable() {
    pic_mask_irq(pit_vector);
    masked = TRUE;
    idt_free_vector(pit_vector);
    pit_vector = 0;
}

void pit_deadline(uint64_t delay) {
    if (!activated) {
        current_tick = 0; 
        activated = TRUE;
        pic_unmask_irq(pit_vector);
    }
    watching = TRUE;
    while (current_tick < delay) {
        __asm__ __volatile__("hlt");
        continue;
    }
    pic_mask_irq(pit_vector);
    masked = TRUE;
    watching = FALSE;
}