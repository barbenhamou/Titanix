#include "../../include/lib/monitor.h"
#include "../../include/lib/mmap.h"
#include "../../include/lib/interrupts.h"
#include "../../include/lib/pit.h"

//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_monitor();
    init_mmap();
    idt_init();
    init_timer(8);
    return 0;
}
