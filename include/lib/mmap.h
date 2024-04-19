#pragma once

#ifndef MMAP_H
#define MMAP_H

#include "types.h"
#include "functions.h"

#define TYPE_USABLE 1
#define TYPE_RESERVED 2
#define TYPE_ACPI_RECLAIMABLE_MEMORY 3
#define TYPE_ACPI_NVS_MEMORY 4
#define TYPE_BAD_MEMORY 5

#define ALIGN(num, align) (((num)+(align)-1) & (-(align))) 

#define MMAP 0x5000
#define PAGE_SIZE 0x1000
#define BIG_PAGE_SIZE 0x200000

extern byte_t* real_mode_start(void);
extern byte_t* real_mode_end(void);
extern void transfer_to_real(void(*)());
extern void load_mmap(void (*)());

typedef struct mmap_entry_t {
    qw_t base;
    qw_t length;
    dw_t type;
    #define TYPE_USABLE 1
    #define TYPE_RESERVED 2
    #define TYPE_ACPI_RECLAIMABLE_MEMORY 3
    #define TYPE_ACPI_NVS_MEMORY 4
    #define TYPE_BAD_MEMORY 5
    dw_t acpi_extended_attr;
} __attribute__((__packed__)) mmap_entry_t;

typedef struct mmap_t {
    dw_t length;
    mmap_entry_t mmap_entries[];
} __attribute__((__packed__)) mmap_t;

typedef struct entries_list_t {
    mmap_entry_t entry;
    struct entries_list_t* next;
    struct entries_list_t* prev;
} entries_list_t;

typedef struct memory_region_t {
    uint64_t base;
    uint64_t length;
} memory_region_t;


byte_t *alloc(uint64_t len);

byte_t* allocate_memory(uint64_t length);

void init_real(void);

void init_mmap(void);

void put_mmap(void);

entries_list_t* make_entries_list();

void print_list(entries_list_t* lst);

mmap_entry_t get_entry_by_index(entries_list_t* lst, uint64_t idx);

uint64_t get_size(entries_list_t* lst);

#endif
