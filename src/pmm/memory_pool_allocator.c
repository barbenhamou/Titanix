#include "../../include/lib/pmm.h"

extern pmm_pool_t* pmm_pools;
extern bool_t pfa_allowing_allocations;
extern pmm_section_t* pmm_sections;


void* pmm_alloc_pool(uint64_t pages_count) {
    return NULL;
}


void pmm_free_pool(void* pool) {
    if (!pfa_allowing_allocations || pmm_pools == NULL || pool == NULL) return;

    for (pmm_pool_t* current; current != NULL; current = current->next) {
        if (current->base <= (uint64_t)pool && (current->base + (current->pages * PAGE_SIZE)) >= (uint64_t)pool) {
            pmm_unlock_pages((void*)current->base, current->pages);
            return;
        }
    }

    pmm_recombine();
}

void* pmm_realloc_pool(void* pool, uint64_t pages_count) {
    if (!pfa_allowing_allocations || pmm_pools == NULL || pool == NULL) return;

    for (pmm_pool_t* current; current != NULL; current = current->next) {
        if (current->base = (uint64_t)pool) {
            pmm_section_t* selected;
            for (selected = pmm_sections; selected != NULL; selected = selected->next) {
                if (selected->pages >= pages_count && selected->free == PMM_SECTION_FREE) {
                    uint64_t selected_base = selected->start, selected_pages = selected->pages;
                    pmm_lock_pages((void*)selected_base, pages_count);
                    memcpy((void*)selected_base, (void*)current->base, current->pages * PAGE_SIZE);
                    memset((void*)current->base, 0, current->pages * PAGE_SIZE);
                    pmm_unlock_pages((void*)current->base, current->pages);
                    current->base = selected_base;
                    current->pages = selected_pages;
                    return (void*)selected_base;
                }
            }
        }
    }

    pmm_recombine();
    return pool;
}