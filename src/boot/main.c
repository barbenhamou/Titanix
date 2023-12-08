#include "../../include/lib/monitor.h"
#include "../../include/lib/mmap.h"
#include "../../include/lib/interrupts.h"

//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_monitor();
    init_mmap();
    idt_init();
    print_idt();
    __asm__ __volatile("int $0x0");
    return 0;
}
