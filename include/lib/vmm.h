#ifndef VMM_H
#define VMM_H

#include "pmm.h"

#define PAGING_PRESENT 1 << 0
#define PAGING_WRITE 1 << 1
#define PAGING_USER 1 << 2
#define PAGING_WRITE_THROUGH 1 << 3
#define PAGING_CACHE_DISABLED 1 << 4
#define PAGING_ACCESSED 1 << 5
#define PAGING_BIG_PAGE 1 << 6
#define PAGING_OS_AVAILABLE 0xE00
#define PAGING_NO_EXECUTE 1 << 63

#define KERNEL_PAGE (PAGING_PRESENT | PAGING_WRITE)
#define USER_PAGE (PAGING_PRESENT | PAGING_WRITE | PAGING_USER)

#define PAGE_ALIGNED __attribute__((aligned(PAGE_SIZE)))

typedef uint64_t page_descriptor_t;

typedef struct paging_indexer_t {
    uint16_t pml4;
    uint16_t pml3;
    uint16_t pml2;
    uint16_t pml1;
} paging_indexer_t;

typedef struct {
    page_descriptor_t entries[512];
} PAGE_ALIGNED page_table_t;

static __attribute__((always_inline)) 
inline
void paging_descriptor_set_address(page_descriptor_t* desc, uint64_t addr) {
    *desc = addr & 0xfffffffffffff000;
}

static __attribute__((always_inline)) 
inline
void paging_set_indexer(paging_indexer_t* indexer, void* addr) {
    uint64_t uaddr = (uint64_t)addr;
    uaddr >>= 12;
    indexer->pml1 = uaddr & 0x1ff;
    uaddr >>= 9;
    indexer->pml2 = uaddr & 0x1ff;
    uaddr >>= 9;
    indexer->pml3 = uaddr & 0x1ff;
    uaddr >>= 9;
    indexer->pml4 = uaddr & 0x1ff;
}

static __attribute__((always_inline))
inline
void* paging_descriptor_get_address(page_descriptor_t* descriptor) {
    return (void*)(*descriptor & 0xffffffffff000);
}

static __attribute__((always_inline))
inline
bool_t paging_descriptor_get_flag(page_descriptor_t* descriptor, uint64_t flag) {
    bool_t bool = (*descriptor) & flag;
    return bool;
}

static __attribute__((always_inline))
inline
void paging_descriptor_set_flag(page_descriptor_t* descriptor, uint64_t flag, bool_t value) {
    *descriptor &= ~flag;
    if (value) {
        *descriptor |= flag;
    }
}

static __attribute__((always_inline))
inline
void paging_descriptor_set_flags(page_descriptor_t* descriptor, uint64_t flags) {
    *descriptor &= ~0x0ffful;
    *descriptor |= flags;
}

static __attribute__((always_inline))
inline
void paging_invlpg(void* page) {
    __asm__ __volatile__ ("invlpg (%0)" : : "r"(page) : "memory");
}

void* paging_map_page(void* virt, void* phys, uint16_t flags);

void paging_unmap_page(void* virt);

void* paging_remap_page(void* old_virt, void* new_virt);

void* paging_walk_page(void* virt);

void* paging_edit_page(void* virt, uint16_t flags);

void paging_load_pml4(page_table_t* pml4);

void* paging_get_pml4();

#endif