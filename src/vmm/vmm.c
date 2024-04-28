#include "../../include/lib/vmm.h"

extern page_table_t* pml4;
page_table_t* pml4;

void* paging_map_page(void* virtual, void* physical, uint16_t flags) {
    flags &= 0x0fff;

    paging_indexer_t indexer;
    paging_set_indexer(&indexer, virtual);
    page_descriptor_t pde;
    pde = pml4->entries[indexer.pml4];
    page_table_t* pdp;
    
    if (!paging_descriptor_get_flag(&pde, flags)) {
        pdp = (page_table_t*)pmm_alloc_page();
        memset((void*)pdp, 0, PAGE_SIZE);
        paging_descriptor_set_address(&pde, (uint64_t)pdp);
        paging_descriptor_set_flag(&pde, PAGING_PRESENT, TRUE);
        pde |= flags;
        pml4->entries[indexer.pml4] = pde;
        paging_map_page((void*)pdp, (void*)((uint64_t)pdp), KERNEL_PAGE);
    } else {
        pdp = (page_table_t*) ((uint64_t)paging_descriptor_get_address(&pde));
        pml4->entries[indexer.pml4] |= flags; 
    }
    pde = pdp->entries[indexer.pml3];
    
    page_table_t* pd;
    if (!paging_descriptor_get_flag(&pde, flags)) {
        pd = (page_table_t*)pmm_alloc_page();
        memset((void*)pd, 0, PAGE_SIZE);
        paging_descriptor_set_address(&pde, (uint64_t)pd);
        paging_descriptor_set_flag(&pde, PAGING_PRESENT, TRUE);
        pde |= flags;
        pdp->entries[indexer.pml3] = pde;
        paging_map_page((void*)pd, (void*)((uint64_t)pd), KERNEL_PAGE);
    } else {
        pd = (page_table_t*) ((uint64_t)paging_descriptor_get_address(&pde));
        pdp->entries[indexer.pml3] |= flags; 
    }
    pde = pd->entries[indexer.pml2];

    page_table_t* pt;
    if (!paging_descriptor_get_flag(&pde, flags)) {
        pt = (page_table_t*)pmm_alloc_page();
        memset((void*)pt, 0, PAGE_SIZE);
        paging_descriptor_set_address(&pde, (uint64_t)pt);
        paging_descriptor_set_flag(&pde, PAGING_PRESENT, TRUE);
        pde |= flags;
        pd->entries[indexer.pml2] = pde;
        paging_map_page((void*)pt, (void*)((uint64_t)pt), KERNEL_PAGE);
    } else {
        pt = (page_table_t*) ((uint64_t)paging_descriptor_get_address(&pde));
        pd->entries[indexer.pml2] |= flags; 
    }

    pde = pt->entries[indexer.pml1];
    paging_descriptor_set_address(&pde, (uint64_t)physical);
    paging_descriptor_set_flag(&pde, KERNEL_PAGE, TRUE);
    paging_descriptor_set_flags(&pde, flags);
    pt->entries[indexer.pml1] = pde;
    return virtual;
}

void* paging_walk_page(void* virtual) {

    paging_indexer_t indexer;
    paging_set_indexer(&indexer, virtual);
    page_descriptor_t pde;
    pde = pml4->entries[indexer.pml4];

    page_table_t* pdp;
    
    if (!paging_descriptor_get_flag(&pde, PAGING_PRESENT)) {
        return NULL;
    }
    

    pdp = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));
    
    pde = pdp->entries[indexer.pml3];
    page_table_t* pd;

    if (!paging_descriptor_get_flag(&pde, PAGING_PRESENT)) {
        return NULL;
    }
        
    pd = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));

    pde = pd->entries[indexer.pml2];
    page_table_t* pt;

    if (!paging_descriptor_get_flag(&pde, PAGING_PRESENT)) {
        return NULL;
    }

    pt = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));

    pde = pt->entries[indexer.pml1];

    void* addr_to =  paging_descriptor_get_address(&pde);
    return addr_to;
}

void paging_unmap_page(void* virtual) {
    paging_indexer_t indexer;
    paging_set_indexer(&indexer, virtual);
    page_descriptor_t pde;

    pde = pml4->entries[indexer.pml4];
    page_table_t* pdp;
    
    if (!paging_descriptor_get_flag(&pde, PAGING_PRESENT))
        return;
        
    pdp = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));
    
    pde = pdp->entries[indexer.pml3];
    page_table_t* pd;

    if (!paging_descriptor_get_flag(&pde, PAGING_PRESENT))
        return;
        
    pd = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));

    pde = pd->entries[indexer.pml2];
    page_table_t* pt;

    if (!paging_descriptor_get_flag(&pde, PAGING_PRESENT))
        return;

    pt = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));

    pde = pt->entries[indexer.pml1];
    pde = 0;
    pt->entries[indexer.pml1] = pde;

    paging_invlpg(virtual);
}


void* paging_edit_page(void* virtual, uint16_t flags) {
    paging_indexer_t indexer;
    paging_set_indexer(&indexer, virtual);

    page_descriptor_t pde;
    pde = pml4->entries[indexer.pml4];
    pml4->entries[indexer.pml4] |= flags;
    page_table_t* pdp = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));
    pdp->entries[indexer.pml3] |= flags;
    pde = pdp->entries[indexer.pml3];
    page_table_t* pd = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));
    pd->entries[indexer.pml2] |= flags;
    pde = pd->entries[indexer.pml2];
    page_table_t* pt = (page_table_t *)((uint64_t)paging_descriptor_get_address(&pde));
    paging_descriptor_set_flags(&pt->entries[indexer.pml1], flags);

    paging_invlpg(virtual);
    return virtual;
}

void* paging_remap_page(void* old, void* new) {
    paging_indexer_t indexer;
    paging_set_indexer(&indexer, old);
    page_descriptor_t old_pde;

    old_pde = pml4->entries[indexer.pml4];
    page_table_t* pdp;
    if (!paging_descriptor_get_flag(&old_pde, PAGING_PRESENT))
        return NULL;
    else
        pdp = (page_table_t *)((uint64_t)paging_descriptor_get_address(&old_pde));
    
    
    old_pde = pdp->entries[indexer.pml3];
    page_table_t* pd;
    if (!paging_descriptor_get_flag(&old_pde, PAGING_PRESENT)) 
        return NULL;
    else
        pd = (page_table_t *)((uint64_t)paging_descriptor_get_address(&old_pde));

    old_pde = pd->entries[indexer.pml2];
    page_table_t* pt;
    if (!paging_descriptor_get_flag(&old_pde, PAGING_PRESENT)) 
        return NULL;
    else
        pt = (page_table_t *)((uint64_t)paging_descriptor_get_address(&old_pde));

    old_pde = pt->entries[indexer.pml1];
    
    paging_unmap_page(old);

    paging_map_page(new, paging_descriptor_get_address(&old_pde), old_pde &= 0x0fff);

    paging_invlpg(old);

    return new;
}

void paging_load_pml4(page_table_t* _pml4) {
    pml4 = _pml4;
}

void* paging_get_pml4() {
    return pml4;
}