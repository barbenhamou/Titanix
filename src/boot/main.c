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

    byte_t* page_for_saving_pmm_sections = (byte_t*)pmm_alloc_page(); //for them not to begin in 0x10000 bc pmm_sections is there
    

    idt_init();
    keyboard_init();
    return 0;
}
