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
#endif