#ifndef BUDDY_H
#define BUDDY_H

#include "mmap.h"

typedef struct buddy_t {
    memory_region_t region;
    uint32_t max_order;
    uint64_t** free_blocks;
} buddy_t;

#define BUDDY_LIMIT 0x1000

uint64_t get_max_order(buddy_t* buddy);
buddy_t* new_buddy(memory_region_t region);
uint64_t get_order(buddy_t* buddy, uint64_t size);


#endif