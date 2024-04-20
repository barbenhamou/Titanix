#include "../../include/lib/kernel_lib.h"
//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_real_mode();
    init_mmap();
    init_monitor();

    entries_list_t* list = make_entries_list();
    pmm_start(list);
    pmm_dump();

    byte_t* out = (byte_t*)pmm_alloc_page();

    //memset((void*)out, 0, PAGE_SIZE);

    pmm_free_page((void*)out);
    idt_init();
    keyboard_init();
    return 0;
}
