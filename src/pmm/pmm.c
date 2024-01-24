#include "../../include/lib/pmm.h"

bool_t pfa_allowing_allocation = FALSE;
extern bool_t pfa_allowing_allocation;

uint64_t free_memory;

pmm_section_t* pmm_sections = NULL;
pmm_pool_t* ppm_pool = NULL;

uint64_t pmm_section_head;
uint64_t data_size;

void pmm_section_manager_create(void* base) {
    if (pfa_allowing_allocation) return;
    pmm_sections = (pmm_section_t*)(base);
    DEBUG("1\n");
    memset(&pmm_sections, 0, data_size);
    DEBUG("1\n");
    pmm_section_head = 1;
}

void pmm_section_manager_reindex() {
    pmm_section_head = 0;
    for (uint64_t i = 0; (i * sizeof(pmm_section_t)) < data_size; ++i) {
        pmm_section_t* current = &pmm_sections[i];
        if (current->prev == NULL && current->next == NULL) {
            pmm_section_head = i;
            return;
        }
    }
}

pmm_section_t* pmm_new_section() {
    if (pmm_section_head * sizeof(pmm_section_t) >= data_size) return NULL;

    pmm_section_t* new_section = &pmm_sections[pmm_section_head];

    if (new_section->prev == NULL || new_section->next == NULL) {
        pmm_section_manager_reindex();
        new_section = &pmm_sections[pmm_section_head];
        if (new_section->prev == NULL || new_section->next == NULL) return NULL;
    }

    pmm_section_head++;
    return new_section;
}

void pmm_delete_section(pmm_section_t* to_delete) {
    to_delete->prev = NULL;
    to_delete->next = NULL;
    to_delete->free = 0;
    to_delete->pages = 0;
}

void pmm_get_free() {
    return free_memory;
}

uint64_t memory_status(uint64_t mmap_signal) {
    //according to the mmap entry type, decide whether its okay to use this region. 1 okay, 0 no.
    switch (mmap_signal) {
    case TYPE_USABLE: 
        return FREE_MEMORY; 

    case TYPE_RESERVED:
        return BAD_MEMORY; 
    
    case TYPE_ACPI_RECLAIMABLE_MEMORY: 
        return BAD_MEMORY; 
    
    case TYPE_ACPI_NVS_MEMORY: 
        return BAD_MEMORY; 

    case TYPE_BAD_MEMORY: 
        return BAD_MEMORY; 

    default:
        return BAD_MEMORY;
    }
}

void pmm_recombine() {
    pmm_section_t* current = NULL;
    uint64_t detected = 0;

    for (uint64_t i = 0; !(detected == 0 && i > 0); ++i) {
        detected = 0;
        for (current = pmm_sections; current != NULL; current = current->next) {
            if (current->next == NULL) {
                break;
            }

            if (current->free != current->next->free) {
                continue;
            }

            detected = 1;
            current->pages += current->next->pages;
            if (current->next->next != NULL) {
                current->next = current->next->next;
                pmm_delete_section(current->next->prev);
                current->next->prev = current;
            } else {
                pmm_delete_section(current->next);
                current->next = NULL;
            }

        }
    }
}

void pmm_recalculate_free_memory() {
    pmm_section_t* current;
    free_memory = 0;
    for (current = pmm_sections; current != NULL; current = current->next) {
        if (current->free == 1) {
            free_memory += current->pages * PAGE_SIZE;
        }
    }
}

static uint64_t estimated_total_memory;

#define MEGABYTE (1024 * 1024)
#define ROUND_OFF 64

void pmm_start() {
    if (pfa_allowing_allocation) return;
    mmap_t* mmap = (mmap_t*)(MMAP);

    data_size = 0;
    pmm_section_head = 0;
    estimated_total_memory = 0;
    free_memory = 0;

    for (uint64_t i = 0; i < mmap->length; ++i) {
        if (mmap->mmap_entries[i].type == TYPE_USABLE) {
            free_memory += mmap->mmap_entries[i].length;
        }
    }

    estimated_total_memory = free_memory / (ROUND_OFF * MEGABYTE);
    if (free_memory % (ROUND_OFF * MEGABYTE)) {
        ++estimated_total_memory;
    }

    estimated_total_memory *= (ROUND_OFF * MEGABYTE);
    data_size = estimated_total_memory / PMM_MAX_HEADER_PROPOTION;
        
    for (uint64_t i = 0; i < mmap->length; ++i) {
        if (mmap->mmap_entries[i].type == TYPE_USABLE) {
            if (mmap->mmap_entries[i].length >= data_size) {
                pmm_section_manager_create((byte_t*)mmap->mmap_entries[i].base);
                break;
            }
        }
    }

    pmm_sections->prev = NULL;
    pmm_sections->next = NULL;
    pmm_sections->start = 0x0000000000000000;
    pmm_sections->pages = MEGABYTE / PAGE_SIZE;
    pmm_sections->free = BAD_MEMORY;

    pmm_section_t* current = pmm_sections;
    for (uint64_t i = 0; i < mmap->length; ++i) {
        if (mmap->mmap_entries[i].base == current->start) {
            continue;
        } else {
            current->next = pmm_new_section();
            current->next->prev = current;
            current->next->next = NULL;
            current->next->start = mmap->mmap_entries[i].base;
            current->next->free = memory_status(mmap->mmap_entries[i].type);
            current->next->pages = mmap->mmap_entries[i].length / PAGE_SIZE;
            if (mmap->mmap_entries[i].length % PAGE_SIZE != 0) current->next->pages++;
            current = current->next;
            if (current->prev->start + (current->prev->pages * PAGE_SIZE) != current->start) {
                current->prev->next = pmm_new_section();
                current->prev->next->prev = current->prev;
                current->prev->next->next = current;
                current->prev->next->start = current->prev->start + (current->prev->pages * PAGE_SIZE);
                current->prev->next->pages = (current->start - current->prev->next->start) / PAGE_SIZE;
                if ((current->start - current->prev->next->start) % PAGE_SIZE != 0) current->prev->next->pages++;
                current->prev->next->free = BAD_MEMORY;
                current->prev = current->prev->next;
            }
        }
    }

    pmm_recombine();

    pmm_recalculate_free_memory();

    pfa_allowing_allocation = TRUE;

    //lock pages now
}

static char* mem_status_discription(uint64_t state) {
    switch (state) {
        case BAD_MEMORY: return "BAD";
        case FREE_MEMORY: return "FREE";
        case USED: return "USED";
        default: return "ERROR!";
    }
}

void pmm_dump() {
    pmm_recalculate_free_memory();
    INFO("\nMemory State:\n");
    INFO("Free memory: %d megabytes\n", free_memory / MEGABYTE);
    INFO("Total memory: %d megabytes\n", estimated_total_memory / MEGABYTE);
    INFO("Regions:\n");
    
    for (pmm_section_t* current = pmm_sections; current != NULL; current = current->next) {
        if (current == NULL) return;
        INFO("\t%x => %dkB/%dmB\n", current->start, (current->pages * PAGE_SIZE) / 1024, (current->pages * PAGE_SIZE) / MEGABYTE);
    }
}