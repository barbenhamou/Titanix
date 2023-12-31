#include "../../include/lib/monitor.h"
#include "../../include/lib/mmap.h"
#include "../../include/lib/interrupts.h"
#include "../../include/lib/pit.h"
#include "../../include/lib/ps2_keyboard.h"

//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_monitor();
    init_mmap();
    idt_init();
    init_timer(500);
    pit_deadline(1193182/10000);
    pit_disable();
    DEBUG("after\n");
    keyboard_init();
    return 0;
}
