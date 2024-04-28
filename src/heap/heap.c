#include "../../include/lib/heap.h"

void* heap_start;
void* heap_end;
heap_segment_header_t* last;
bool_t heap_initialized = FALSE;

void combine_with_next(heap_segment_header_t* seg) {
    if (seg->next == NULL) {
        return NULL;
    }

    if (!seg->next->free) {
        return NULL;
    }

    if (seg->next == last) {
        last = seg;
    }

    if (seg->next->next != NULL) {
        seg->next->next->prev = seg;
    }

    seg->length = seg->length + seg->next->length + sizeof(heap_segment_header_t);

    seg->next = seg->next->next;
}

void combine_with_prev(heap_segment_header_t* seg) {
    if (seg->prev != NULL && seg->prev->free) {
        combine_with_next(seg->prev);
    }
}

heap_segment_header_t* split_header(heap_segment_header_t* seg, uint64_t split_at) {
    if (split_at < 0x10) {
        return NULL;
    }

    long long length = seg->length - split_at - sizeof(heap_segment_header_t); 

    if (length < 0x10) {
        return NULL;
    }

    heap_segment_header_t* new_header = (heap_segment_header_t*)((uint64_t)seg + split_at + sizeof(heap_segment_header_t));

    if (new_header->next != NULL) {
        seg->next->prev = new_header;
    }

    new_header->next = seg->next;
    seg->next = new_header;
    new_header->prev = seg;
    new_header->free = seg->free;
    new_header->length = length;
    seg->length = split_at;

    if (last == seg) {
        last = new_header;
    }

    return new_header;
}

void init_heap(void* address, uint64_t amount_pages) {
    if (heap_initialized) {
        return;
    }

    void* ptr = address;
    void* page;
    for (uint64_t i = 0; i < amount_pages; ++i) {
        page = pmm_alloc_page();
        paging_map_page(ptr, page, KERNEL_PAGE);
        paging_invlpg(ptr);
        paging_invlpg(page);
        ptr = (void*)((uint64_t)ptr + PAGE_SIZE);
    }

    uint64_t length = amount_pages * PAGE_SIZE;

    heap_start = address;

    heap_end = (void*)((uint64_t)address + length);

    heap_segment_header_t* begin = (heap_segment_header_t*)address;


    begin->length = length - sizeof(heap_segment_header_t);

    begin->free = TRUE;

    begin->next = NULL;

    begin->prev = NULL;

    last = begin;

    heap_initialized = TRUE;
}

void expand_heap(uint64_t length) {
    if (length % PAGE_SIZE) {
        length -= length % PAGE_SIZE;
        length += PAGE_SIZE;
    }

    uint64_t amount_pages = length / PAGE_SIZE;

    heap_segment_header_t* end_header = (heap_segment_header_t*)heap_end;

    for (uint64_t i = 0; i < amount_pages; ++i) {
        paging_map_page(heap_end, pmm_alloc_page(), KERNEL_PAGE);
        heap_end = (void*)((uint64_t)heap_end + PAGE_SIZE);
    }

    end_header->free = TRUE;
    end_header->prev = last;
    last->next = end_header;
    last = end_header;
    end_header->next = NULL;
    end_header->length = length - sizeof(heap_segment_header_t);
    combine_with_next(end_header);
}

void free(void* address) {
    heap_segment_header_t* seg = (heap_segment_header_t*)address - 1;
    seg->free = TRUE;
    combine_with_next(seg);
    combine_with_prev(seg);
}

void* malloc(uint64_t length) {
    if (!heap_initialized) {
        init_heap((void *)0x10000f000, 256);
    }

    if (length % 0x10) {
        length -= length % 0x10;
        length += 0x10;
    }

    if (length == 0) {
        return 0;
    }

    heap_segment_header_t* selected = (heap_segment_header_t*)heap_start;

    while (TRUE) {
        if (selected->free) {
            if (selected->length > length) {
                split_header(selected, length);
                selected->free = FALSE;
                return (void*)((uint64_t)selected + sizeof(heap_segment_header_t));
            } 

            if (selected->length == length) {
                selected->free = FALSE;
                return (void*)((uint64_t)selected + sizeof(heap_segment_header_t));
            }
        }

        if (selected->next == NULL) {
            break;
        }

        selected = selected->next;
    }

    expand_heap(length);

    return malloc(length);
}

void* realloc(void* address, uint64_t size) {
    heap_segment_header_t* seg = (heap_segment_header_t*)address - 1;
    uint64_t length = seg->length;

    void* header = malloc(size);

    memcpy(header, address, length);
    free(address);
}

void* calloc(uint64_t num_of_elements, uint64_t size_of_elements) {
    uint64_t real_length = num_of_elements * size_of_elements;

    void* allocated = malloc(real_length);

    memset(allocated, 0, real_length);

    return allocated;
}
