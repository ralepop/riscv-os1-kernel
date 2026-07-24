#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/memory_allocator.h"
#include "../h/scheduler.h"

MemoryAllocator mem;
extern "C" void interrupt_routine();
extern "C" void interrupt_return();
extern "C" struct registers register_state;


void main() {

    // kada se desi prekid izvrsavamo kod cija se adresa nalazi u funkciji interrupt_routine
    __asm__ volatile("csrw stvec, %0" :: "r"(interrupt_routine));
    mem = MemoryAllocator();
    // TODO scheduler i semafori

    // gasimo spoljasnje hardverske prekide
    __asm__ volatile("csrw sie, %0" :: "r"((uint64)0b10));
}