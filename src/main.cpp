#include "../h/MemoryAllocator.h"
#include "../lib/console.h"

extern "C" void interrupt() {

}

extern MemoryAllocator mem;
MemoryAllocator mem;

void run_allocator_test() {
    __putc('T'); __putc('E'); __putc('S'); __putc('T'); __putc('\n'); 

    void *p1 = mem.mem_alloc(64); // ima deljenje fragmenata
    void *p2 = mem.mem_alloc(104); // nema deljenja fragmenata: 104 + 24 (sizeof(Fragment)) = 128
    void *p3 = mem.mem_alloc(64);

    if (p1 == nullptr || p2 == nullptr || p3 == nullptr) {
        __putc('E'); __putc('R'); __putc('R'); __putc('O'); __putc('R'); __putc('\n');
        return;
    }

    mem.mem_free(p2);
    mem.mem_free(p1);
    mem.mem_free(p3);

    __putc('O'); __putc('K'); __putc('\n'); 

}

void main() {

    asm volatile("csrw stvec,%0"::"r"(interrupt));
    mem = MemoryAllocator();

    run_allocator_test();
}