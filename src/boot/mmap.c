#include "../../include/lib/mmap.h"
#include "../../include/lib/monitor.h"

void init_real_mode(void) {
    memcpy((byte_t*)0x4000, real_mode_start, real_mode_end - real_mode_start);
}

void init_mmap() {
    transfer_to_real(load_mmap);
}

byte_t *alloc(uint64_t length) {
    uint64_t len = ALIGN(length, PAGE_SIZE);
    length = len;
    mmap_t *mmap = (mmap_t*)MMAP;
    uint32_t size = mmap->length;
    uint32_t i, chosen_chunk = size;

    for (i = 0; i < size; ++i) {
        if (mmap->mmap_entries[i].type == TYPE_USABLE && mmap->mmap_entries[i].length > length) {
            chosen_chunk = i;
            break;
        }
    }

    byte_t *allocated = (byte_t*)ALIGN(mmap->mmap_entries[chosen_chunk].base + mmap->mmap_entries[chosen_chunk].length - length, PAGE_SIZE);

    mmap->mmap_entries[chosen_chunk].length -= length;
    INFO("Allocted %x bytes from %x; %x left in this section.\n", length, mmap->mmap_entries[chosen_chunk].base, mmap->mmap_entries[chosen_chunk].length);

    if (allocated != 0) {
        memset(allocated, 0, length);
    }

    return allocated;
}

void put_mmap() {
    mmap_t *mmap = (mmap_t*)MMAP;
    uint32_t i, len = mmap->length;
    INFO("_________________Printing mmap_________________\n");
    for (i = 0; i < len; ++i) {
        INFO("Entry number %d attributes:\n", i);
        INFO("base address: %x\n\tlength: %x\n\ttype: %x\n", mmap->mmap_entries[i].base, mmap->mmap_entries[i].length, mmap->mmap_entries[i].type);
        puts("\n");
    }
    INFO("_______________________________________________\n");
}

