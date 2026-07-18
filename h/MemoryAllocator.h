#include "../lib/hw.h"

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

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