#include "../../include/lib/pmm.h"

extern bool_t pfa_allowing_allocations;
extern uint64_t free_memory;
extern pmm_section_t* pmm_sections;
extern pmm_pool_t* pmm_pools;

void pmm_section_split(pmm_section_t* target, uint64_t split_idx) {
    if (split_idx >= (target->start + target->pages * PAGE_SIZE)) {
        return;
    }

    if (target->next != NULL) {
        target->next->prev = pmm_new_section();
        target->next->prev->prev = target;
        target->next->prev->next = target->next;
        target->next = target->next->prev;
    } else {
        target->next = pmm_new_section();
        target->next->prev = target;
    }

    target->next->free = target->free;
    target->next->pages = target->pages - ((split_idx - target->start) / PAGE_SIZE);
    target->next->start = split_idx;
    target->pages -= ((target->start + (target->pages * PAGE_SIZE)) - split_idx) / PAGE_SIZE;
}

void pmm_section_combine_next(pmm_section_t* target) {
    if (target->next == NULL) return;
    if (target->free != target->next->free) return;

    target->next->start = target->start;
    target->next->pages += target->pages;
    target->next->prev = target->prev;
    target->prev->next = target->next;
    pmm_delete_section(target);
}

void pmm_section_combine_prev(pmm_section_t* target) {
    if (target->prev == NULL) return;
    if (target->free != target->prev->free) return;

    target->prev->pages += target->pages;
    target->prev->next = target->next;
    target->next->prev = target->prev;
    pmm_delete_section(target);
}

void* pmm_alloc_page() {
    if (!pfa_allowing_allocations) return NULL;

    uint64_t base = 0;

    pmm_section_t* selected;

    for (selected = pmm_sections; selected != NULL; selected = selected->next) {
        if (selected->pages == 0) {
            selected->next->prev = selected->prev;
            selected->prev->next = selected->next;
            pmm_section_t* old = selected;
            selected = selected->prev;
            pmm_delete_section(old);
            continue;
        }

        if (selected->free == PMM_SECTION_FREE) {
            base = selected->start;
            if (selected->prev->free == PMM_SECTION_USED) {
                selected->prev->pages += 1;
                selected->pages -= 1;
                selected->start += PAGE_SIZE;
            } else {
                pmm_section_split(selected, selected->start + PAGE_SIZE);
                selected->free = PMM_SECTION_USED;
            }
            break;
        }
    }

    if (base != 0) {
        free_memory -= PAGE_SIZE;
    }

    return (void*)base;
}

void pmm_free_page(void* page) {
    if (!pfa_allowing_allocations) return;

    pmm_section_t* selected;
    for (selected = pmm_sections; selected != NULL; selected = selected->next) {
        if (selected->start == (uint64_t)page) {
            if (selected->free == PMM_SECTION_FREE) return;
            if (selected->pages == 1) {
                if (selected->prev->free == PMM_SECTION_FREE && selected->next->free == PMM_SECTION_FREE) {
                    pmm_section_t* prev = selected->prev;
                    pmm_section_combine_next(selected);
                    pmm_section_combine_prev(prev);
                    return;

                } else if (selected->prev->free == PMM_SECTION_FREE) {
                    pmm_section_combine_prev(selected);
                    return;

                } else if (selected->next->free == PMM_SECTION_FREE) {
                    pmm_section_combine_next(selected);
                    return;

                } else {
                    selected->free = PMM_SECTION_FREE;
                    return;
                }
            } else {
                if (selected->prev->free == PMM_SECTION_FREE) {
                    selected->prev->pages += 1;
                    selected->start += PAGE_SIZE;
                    selected->pages -= 1;
                    return;

                } else {
                    pmm_section_split(selected, selected->start + PAGE_SIZE);
                    return;
                }
            }
        }

        if (selected->start < (uint64_t)page && selected->next->start > (uint64_t)page) {
            if (selected->free == PMM_SECTION_FREE) return;
            if ((selected->next->start - PAGE_SIZE) == (uint64_t)page) {
                if (selected->next->free == PMM_SECTION_FREE) {
                    selected->next->start -= PAGE_SIZE;
                    selected->next->pages += 1;
                    selected->pages -= 1;
                } else {
                    pmm_section_split(selected, (uint64_t)page);
                    selected = selected->next;
                    pmm_section_split(selected, selected->start + PAGE_SIZE);
                    selected->free = PMM_SECTION_FREE;
                }
            }
        }
    }
    free_memory += PAGE_SIZE;
}

void pmm_lock_page(void* page) {
    if (!pfa_allowing_allocations) return;

    pmm_section_t* selected;
    for (selected = pmm_sections; selected != NULL; selected = selected->next) {
        if (selected->pages == 1) {
            if (selected->start == (uint64_t)page) {
                selected->free = PMM_SECTION_USED;
                if (selected->prev->free == PMM_SECTION_USED && selected->next->free == PMM_SECTION_USED) {
                    pmm_section_t* prev = selected->prev;
                    pmm_section_combine_next(selected);
                    pmm_section_combine_prev(prev);
                    return;
                } else if (selected->prev->free == PMM_SECTION_USED) {
                    pmm_section_combine_prev(selected);
                    return;
                } else if (selected->next->free == PMM_SECTION_USED) {
                    pmm_section_combine_next(selected);
                    return;
                }
                return;
            }
        }

        if (selected->start == (uint64_t)page) {
            if (selected->free == PMM_SECTION_USED) return;
            if (selected->prev->free == PMM_SECTION_USED) {
                selected->prev->pages += 1;
                selected->pages -= 1;
                selected->start += PAGE_SIZE;
                return;
            } else {
                pmm_section_split(selected, selected->start + PAGE_SIZE);
                selected->free = PMM_SECTION_USED;
            }
        }

        if (selected->start < (uint64_t)page && selected->next->start > (uint64_t)page) {
            if (selected->free == PMM_SECTION_USED) return;
            if ((selected->next->start - PAGE_SIZE) == (uint64_t)page) {
                if (selected->next->free == PMM_SECTION_USED) {
                    selected->next->start -= PAGE_SIZE;
                    selected->next->pages += 1;
                    selected->pages -= 1;
                } else {
                    pmm_section_split(selected, (uint64_t)page);
                    selected = selected->next;
                    selected->free = PMM_SECTION_USED;
                }

            } else {
                pmm_section_split(selected, (uint64_t)page);
                selected = selected->next;
                pmm_section_split(selected, selected->start + PAGE_SIZE);
                selected->free = PMM_SECTION_USED;
            }
        }
    }
}

void pmm_unlock_page(void* page) {
    if (!pfa_allowing_allocations) return;

    pmm_free_page(page);
}

void pmm_lock_pages(void* page, uint64_t count) {
    if (!pfa_allowing_allocations) return;

    for (uint64_t i = 0; i < count; ++i) {
        pmm_lock_page((void*)((uint64_t)page + i * PAGE_SIZE));
    }
}

void pmm_unlock_pages(void* page, uint64_t count) {
    if (!pfa_allowing_allocations) return;

    for (uint64_t i = 0; i < count; ++i) {
        pmm_unlock_page((void*)((uint64_t)page + i * PAGE_SIZE));
    }
}

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