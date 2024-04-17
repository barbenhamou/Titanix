#include "../../include/lib/mmap.h"

entries_list_t* make_entries_list() {
    mmap_t* mmap = (mmap_t*)MMAP;

    // entries_list_t* initialized = (entries_list_t*)(&(entries_list_t){mmap->mmap_entries[0], NULL, NULL});

    // entries_list_t* next1 = (entries_list_t*)(&(entries_list_t){mmap->mmap_entries[1], NULL, initialized});
    // initialized->next = next1;

    // entries_list_t* next2 = (entries_list_t*)(&(entries_list_t){mmap->mmap_entries[2], NULL, next1});
    // initialized->next->next = next2;

    // entries_list_t* next3 = (entries_list_t*)(&(entries_list_t){mmap->mmap_entries[3], NULL, next2});
    // initialized->next->next->next = next3;

    // entries_list_t* next4 = (entries_list_t*)(&(entries_list_t){mmap->mmap_entries[4], NULL, next3});
    // initialized->next->next->next->next = next4;

    // entries_list_t* next5 = (entries_list_t*)(&(entries_list_t){mmap->mmap_entries[5], NULL, next4});
    // initialized->next->next->next->next->next = next5;

    mmap_entry_t last_entry = {0x100000000, 0x80000000, TYPE_USABLE, 0b1};

    // entries_list_t* next6 = (entries_list_t*)(&(entries_list_t){last_entry, NULL, next5});
    // initialized->next->next->next->next->next->next = next6;
    byte_t* allocating = alloc(280);
    //entries_list_t* initialized = (entries_list_t*)(alloc(40));
    entries_list_t* initialized = (entries_list_t*)(allocating);
    //initialized->next = (entries_list_t*)(alloc(40));
    initialized->next = (entries_list_t*)(allocating+40);
    //initialized->next->next = (entries_list_t*)(alloc(40));
    initialized->next->next = (entries_list_t*)(allocating+80);
    //initialized->next->next->next = (entries_list_t*)(alloc(40));
    initialized->next->next->next = (entries_list_t*)(allocating+120);
    // initialized->next->next->next->next = (entries_list_t*)(alloc(40));
    initialized->next->next->next->next = (entries_list_t*)(allocating+160);
    // initialized->next->next->next->next->next = (entries_list_t*)(alloc(40));
    initialized->next->next->next->next->next = (entries_list_t*)(allocating+200);
    // initialized->next->next->next->next->next->next = (entries_list_t*)(alloc(40));
    initialized->next->next->next->next->next->next = (entries_list_t*)(allocating+240);

    initialized->entry = mmap->mmap_entries[0];
    initialized->prev = NULL;
    
    initialized->next->entry = mmap->mmap_entries[1];
    initialized->next->prev = initialized;

    initialized->next->next->entry = mmap->mmap_entries[2];
    initialized->next->next->prev =  initialized->next;

    initialized->next->next->next->entry = mmap->mmap_entries[3];
    initialized->next->next->next->prev = initialized->next->next;

    initialized->next->next->next->next->entry = mmap->mmap_entries[4];
    initialized->next->next->next->next->prev = initialized->next->next->next;

    initialized->next->next->next->next->next->entry = mmap->mmap_entries[5];
    initialized->next->next->next->next->next->prev = initialized->next->next->next->next;

    initialized->next->next->next->next->next->next->entry = last_entry;
    initialized->next->next->next->next->next->next->prev = initialized->next->next->next->next->next;

    return initialized;
}

void print_list(entries_list_t* lst) {
    entries_list_t* another = lst;
    uint32_t i;
    INFO("_________________Printing list_________________\n");
    for (i = 0; i < 7; ++i) {
        INFO("Entry number %d attributes:\n", i);
        INFO("base address: %x\n\tlength: %x\n\ttype: %x\n", lst->entry.base, lst->entry.length, lst->entry.type);
        puts("\n");
        lst = lst->next;
    }
    INFO("_______________________________________________\n");
}

mmap_entry_t get_entry_by_index(entries_list_t* lst, uint64_t idx) {
    entries_list_t* helper = lst;
    for (int i = 0; i < idx; ++i) {
        helper = helper->next;
    }
    return helper->entry;
}

uint64_t get_size(entries_list_t* lst) {
    entries_list_t* helper = lst;
    uint64_t i = 0;

    for (i = 0; helper != NULL; helper = helper->next, ++i);
   
    return i;
}