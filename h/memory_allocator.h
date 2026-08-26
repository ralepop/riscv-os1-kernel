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

    void *mem_alloc(size_t size);
    int mem_free(void *ptr);
    size_t mem_get_free_space();
    size_t mem_get_largest_free_block();

    Fragment *head;
};

extern MemoryAllocator mem;

#endif