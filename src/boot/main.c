#include "../../include/lib/kernel_lib.h"
//extern void transfer_to_real(void(*)());
//extern void funky_func(void);

int main()
{
    init_real_mode();
    init_mmap();

    entries_list_t* list = make_entries_list();
    pmm_start(list);

    //print_list(list);
    byte_t* page_for_saving_pmm_sections = (byte_t*)pmm_alloc_page(); //for them not to begin in 0x10000 bc pmm_sections is there

    page_table_t* pml4_ = paging_load_kernel_map(list);

    byte_t *p1 = (byte_t*)pmm_alloc_page(), *pos1 = 0x1000, *pos2 = 0x4000;
    *pos2 = 2;

    paging_map_page((void *)pos1, (void *)p1, KERNEL_PAGE);
    paging_map_page((void *)pos2, (void *)p1, KERNEL_PAGE);
    paging_invlpg(pos1);
    paging_invlpg(pos2);
    paging_invlpg(p1);

    *pos1 = 1;

    DEBUG("%d\n", *pos2);

    *pos2 = 4;

    DEBUG("%d\n", *pos1);

    byte_t* out = (byte_t*)calloc(0x1000, sizeof(byte_t));

    memset((void*)out, 3, 0x1000 * sizeof(byte_t));

    DEBUG("%x\n", *out);

    init_monitor();
    init_idt();
    init_keyboard();
    return 0;
}
