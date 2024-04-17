#include "../../include/lib/buddy.h"

buddy_t* new_buddy(memory_region_t region) {
    byte_t* allocated = alloc(sizeof(buddy_t));
    buddy_t* buddy = (buddy_t*)allocated;

    if (buddy == NULL) {
        ERROR("Buddy creation gone wrong!, fix it!");
        return NULL;
    }


    buddy->region = region;
    buddy->max_order = (uint32_t)__builtin_ctzll(region.length);
    buddy->free_blocks = (uint64_t**)alloc(buddy->max_order * sizeof(uint64_t*));

    if (buddy->free_blocks == NULL) {
        ERROR("Buddy blocks pointer creation gone wrong!, fix it!");
        return NULL;
    }

    for (uint64_t i = 0; i < buddy->max_order; ++i) {
        buddy->free_blocks[i] = (uint64_t*)alloc(i * sizeof(uint64_t));

        if (buddy->free_blocks[i] == NULL) {
            ERROR("Buddy blocks creation gone wrong!, fix it!");
            return NULL;
        }
        
        buddy->free_blocks[i][0] = 0;
    }

    INFO("New Buddy created succesfully");

    return buddy;
}

uint64_t get_max_order(buddy_t* buddy) {
    return BUDDY_LIMIT << buddy->max_order;
}

uint64_t get_order(buddy_t* buddy, uint64_t size) {
    uint64_t max_order_block = get_max_order(buddy);
    if (size > max_order_block) {
        return -1;
    }

    uint64_t next_order = 1;
    while ((max_order_block >> next_order) >= size) {
        ++next_order;
    }

    return next_order - 1;
}

uint64_t* get_free_block(buddy_t* buddy, uint32_t order) {

}