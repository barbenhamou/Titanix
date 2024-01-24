#ifndef PMM_H
#define PMM_H

#include "mmap.h"

#define BAD_MEMORY 0
#define FREE_MEMORY 1
#define USED 2

typedef struct pmm_section_t {
    struct pmm_section_t* prev;
    struct pmm_section_t* next;
    uint64_t start;
    uint64_t pages;
    uint64_t free;
} pmm_section_t;

typedef struct pmm_pool_t {
    struct pmm_pool_t* next;
    uint64_t pages;
    uint64_t base;
} pmm_pool_t;

#define PMM_MAX_HEADER_PROPOTION 64

extern bool_t pfa_allowing_allocation;

void pmm_section_manager_create(void* base);
void pmm_section_manager_reindex();
pmm_section_t* pmm_new_section();
void pmm_delete_section(pmm_section_t* to_delete);
void pmm_get_free();
uint64_t memory_status(uint64_t mmap_signal);
void pmm_recombine();
void pmm_recalculate_free_memory();
void pmm_start();
static char* mem_status_discription(uint64_t state);
void pmm_dump();
#endif