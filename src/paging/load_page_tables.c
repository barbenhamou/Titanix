#include "../../include/lib/paging.h"

extern page_table_t* pml4;
extern void sync_cr3(void* pml);

void sync(void* pml4__) {

    __asm__ __volatile__ ("mov %0, %%cr3" : : "a"(pml4__));
}

page_table_t* paging_load_kernel_map(entries_list_t* list) {
    pml4 =  (page_table_t*)pmm_alloc_page();
    
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

    paging_map_page((void *)0xb8000, (void *)0xb8000, KERNEL_PAGE);

    void* pml4__ = paging_walk_page((void *)pml4);

    sync(pml4__);

    return pml4;
}

void* get_kernel_physical() {
    return (void*)(0x7c00);
}

uint64_t get_kernel_virtual_offset() {
    return 0xffffffff80000000 - (uint64_t)get_kernel_physical();
}