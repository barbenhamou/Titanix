#ifndef HEAP_H
#define HEAP_H

#include "paging.h"

typedef struct heap_segment_header_t {

    uint64_t length
    heap_segment_header_t* prev;
    heap_segment_header_t* next;
    bool_t free;

} heap_segment_header_t;

heap_segment_header_t* combine_with_next(heap_segment_header_t* seg);

heap_segment_header_t* combine_with_prev(heap_segment_header_t* seg);

heap_segment_header_t* split_header(heap_segment_header_t* seg, uint64_t split_at);

void init_heap(void* address, uint64_t amount_pages);

void expand_heap(uint64_t length);

void free(void* address);

void* malloc(uint64_t length);

void* realloc(void* address, uint64_t size);

void* calloc(uint64_t num_of_elements, uint64_t size_of_elements);

#endif