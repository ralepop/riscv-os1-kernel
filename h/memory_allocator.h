#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include "../lib/hw.h"

struct Fragment {
    uint64 number_of_blocks;
    Fragment *next;
    Fragment *prev;
};

class MemoryAllocator {
public:
    MemoryAllocator();
    void* mem_alloc(size_t size);
    int mem_free(void *ptr);
    Fragment *head;
};

#endif