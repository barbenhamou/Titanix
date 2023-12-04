#include "../../include/lib/mmap.h"
#include "../../include/lib/monitor.h"

extern byte_t* real_mode_start(void);
extern byte_t* real_mode_end(void);
extern void transfer_to_real(void(*)());
extern void load_mmap(void (*)());

void init_real_mode(void) {
    memcpy((byte_t*)0x4000, real_mode_start, real_mode_end - real_mode_start);
}

void init_mmap() {
    transfer_to_real(load_mmap);
}

byte_t *alloc (uint64_t length) {
    uint64_t len = ALIGN(length, PAGE_SIZE);
    mmap_t *mmap = (mmap_t*)MMAP;
    uint32_t size = mmap->length;
    uint32_t i, chosen_chunk = size;

    for (i = 0; i < size; ++i) {
        if (mmap->mmap_entries[i].type == TYPE_USABLE && mmap->mmap_entries[i].length > len) {
            chosen_chunk = i;
            break;
        }
    }

    byte_t *allocated = (byte_t*)ALIGN(mmap->mmap_entries[chosen_chunk].base, PAGE_SIZE);

    //Alignment remainings.
    uint64_t remainings = (uint64_t)allocated - mmap->mmap_entries[chosen_chunk].base;
    mmap->mmap_entries[chosen_chunk].length -= len + remainings;
    mmap->mmap_entries[chosen_chunk].base += len + remainings;
    DEBUG("Allocted %x bytes from %x; %x left in this section (%x).\n", length, mmap->mmap_entries[chosen_chunk].length, mmap->mmap_entries[chosen_chunk].base);

    if (allocated != 0) {
        memset(allocated, 0, len);
    }

    return allocated;
}

void put_mmap() {
    mmap_t *mmap = (mmap_t*)MMAP;
    uint32_t i, len = mmap->length;
    INFO("_________________Printing mmap_________________\n");
    for (i = 0; i < len; ++i) {
        INFO("Entry number %d, at %x\n", i, (mmap->mmap_entries[i].base));
        INFO("Entry attributes:\n\tid: %d\n\tbase address: %x\n\tlength: %x\n\ttype: %x\n",
                i, mmap->mmap_entries[i].base, mmap->mmap_entries[i].length, mmap->mmap_entries[i].type);
    }
    INFO("_______________________________________________\n");
}

