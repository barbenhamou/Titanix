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

#define ALIGN(num, align) (((num)+(align) - 1) & (-(align)))

#define MMAP 0x5000
#define PAGE_SIZE 0x1000
#define BIG_PAGE_SIZE 0x200000

byte_t *alloc(uint64_t len);
void init_real(void);
void init_mmap(void);
void put_mmap(void);

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

#endif
