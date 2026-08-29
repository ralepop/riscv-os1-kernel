#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/memory_allocator.h"
#include "../h/scheduler.h"

extern "C" {
    #include "../h/syscall_c.h"
}

#include "../test/Threads_CPP_API_test.hpp"

extern MemoryAllocator mem;
MemoryAllocator mem;

extern Scheduler scheduler;

extern sem_t wait_for_char;
sem_t wait_for_char;

extern "C" void interrupt_routine();
extern "C" void interrupt_return();
extern "C" struct registers register_state;

void userMain();

void main() {

    // kada se desi prekid izvrsavamo kod cija se adresa nalazi u funkciji interrupt_routine
    __asm__ volatile("csrw stvec, %0" :: "r"(interrupt_routine));
    mem = MemoryAllocator();
    scheduler = Scheduler();
    
    // gasimo spoljasnje hardverske prekide
    __asm__ volatile("csrw sie, %0" :: "r"((uint64)0b10));


    scheduler.thread_create(nullptr, (void(*)(void*))(&Threads_CPP_API_test), nullptr, mem.mem_alloc(DEFAULT_STACK_SIZE));

    scheduler.curr_active_thread = scheduler.pick_next();
    register_state = scheduler.curr_active_thread->context;
    interrupt_return();


}