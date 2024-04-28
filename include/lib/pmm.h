#ifndef PMM_H
#define PMM_H

#include "mmap.h"

static inline char* memtype_string(uint32_t type) {
    switch (type) {
        case 1: return "free";
        case 2: return "used";
        case 3: return "mmio";
        case 4: return "mmio";
        case 5: return "bad";
    }
    return "unidentifiable";
}

#define PMM_SECTION_USED    0
#define PMM_SECTION_FREE    1
#define PMM_SECTION_BAD     2

typedef struct _pmm_section {
    struct _pmm_section* prev;
    struct _pmm_section* next;
    uint64_t start;
    uint64_t pages;
    uint64_t free;
} pmm_section_t;

typedef struct _pmm_pool {
    struct _pmm_pool* next;
    uint64_t pages;
    uint64_t base;
} pmm_pool_t;

#define MAX_PMM_HEADER_PROPORTION   64
// uses about 1 MiB of memory for 64 MiB of memory under maximum load
void pmm_section_manager_reindex();

pmm_section_t* pmm_new_section();

void pmm_delete_section(pmm_section_t* section);

void pmm_section_split(pmm_section_t* target, uint64_t split_idx);

void pmm_section_combine_next(pmm_section_t* target);

void pmm_section_combine_prev(pmm_section_t* target);

void* pmm_alloc_page(void);

void pmm_free_page(void* page);

void* pmm_alloc_pool(uint64_t page_count);

void* pmm_realloc_pool(void* pool, uint64_t page_count);

void pmm_free_pool(void* pool);

void pmm_lock_page(void* page);

void pmm_unlock_page(void* page);

void pmm_lock_pages(void* page, uint64_t count);

void pmm_unlock_pages(void* page, uint64_t count);

uint64_t pmm_get_free_memory();

void pmm_start(entries_list_t* list);

void pmm_dump();

#endif