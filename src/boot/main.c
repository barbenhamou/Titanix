#include "../../include/lib/kernel_lib.h"
//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_real_mode();
    init_mmap();
    init_monitor();

    put_mmap();

    entries_list_t* list = make_entries_list();
    //print_list(list);


    //buddy_t* buddy = new_buddy((memory_region_t){0x0, 2048});
    byte_t* ptr = alloc(0x99900);
    //pmm_start(list);

    idt_init();
    keyboard_init();

    
    //pmm_dump();
    return 0;
}
