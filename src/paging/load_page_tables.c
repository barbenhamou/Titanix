#include "../../include/lib/paging.h"

page_table_t* paging_load_kernel_map(entries_list_t* list) {
    page_table_t* pml4 =  (page_table_t*)pmm_alloc_page();
    
    entries_list_t* selected = list;

    uint64_t n = get_size(list);

    memset(pml4, 0, PAGE_SIZE);

    for (selected = list; selected != NULL; selected = selected->next) {
        if (selected->entry.type != 3) {
            for (uint64_t i = selected->entry.base; i < selected->entry.base + selected->entry.length; i += PAGE_SIZE) {
                paging_map_page((void *)i, (void *)i, KERNEL_PAGE);
                paging_map_page((void *)(i + VIRTUAL_OFFSET), (void *)i, KERNEL_PAGE);
            }
        }
    }

    uint64_t kernel_physical_addr =  (uint64_t)get_kernel_physical();

    for (uint64_t i = 0; i < (uint64_t)&load_max - (kernel_physical_addr + get_kernel_virtual_offset()); i += PAGE_SIZE) {
        paging_map_page((void *)(kernel_physical_addr + get_kernel_virtual_offset() + i), (void *)(kernel_physical_addr + i), KERNEL_PAGE);
    }

    __asm__ __volatile__("mov %0, %%cr3" : : "a"(paging_walk_page(pml4)));
}

