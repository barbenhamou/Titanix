#include "../../include/lib/kernel_lib.h"
//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_monitor();
    init_mmap();
    idt_init();
    //put_mmap();
    //DEBUG("reached here!\n");
    //keyboard_init();
    //pmm_start();
    //pmm_dump();
    return 0;
}
