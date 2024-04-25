#ifndef PAGING_H
#define PAGING_H

#include "vmm.h"

#define VIRTUAL_OFFSET 0xffff800000000000
#define VIRTUAL_LOAD 0xffffffff80000000


extern uint64_t load_base;
extern uint64_t load_max;

page_table_t* paging_load_kernel_map(entries_list_t* list);

inline void paging_sync_cr3(page_table_t* pml4) {
    __asm__ __volatile__ ("mov %0, %%cr3" : : "a"(pml4));
}
void* get_kernel_physical() {
    return (void*)(0x7c00);
}

uint64_t get_kernel_virtual_offset() {
    return 0xffffffff80000000 - (uint64_t)get_kernel_physical();
}

#endif