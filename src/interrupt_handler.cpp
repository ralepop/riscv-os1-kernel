#include "../h/interrupt_handler.h"
#include "../h/memory_allocator.h"
#include "../lib/hw.h"

extern MemoryAllocator mem;
extern "C" void interrupt_return();

extern "C" struct registers register_state = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

extern "C" uint64 kernel_stack;
uint64 kernel_stack;

extern "C" void interrupt_handler() {

    uint64 scause;
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));


}