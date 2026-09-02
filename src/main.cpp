#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/memory_allocator.h"
#include "../h/scheduler.h"
#include "../h/semaphore.h"

extern "C" {
    #include "../h/syscall_c.h"
}

extern MemoryAllocator mem;
MemoryAllocator mem;

extern Scheduler scheduler;

extern sem_t wait_for_char;
sem_t wait_for_char;

extern "C" void interrupt_routine();
extern "C" void interrupt_return();
extern "C" struct registers register_state;

void userMain();

void idle_wrapper(void* arg) {
    while (true) {
        thread_dispatch();
    }
}

void cpp_test_wrapper(void* arg) {
    void (*test_func)() = (void (*)())arg;
    test_func();
    EXIT
}

void main() {

    // kada se desi prekid izvrsavamo kod cija se adresa nalazi u funkciji interrupt_routine
    __asm__ volatile("csrw stvec, %0" :: "r"(interrupt_routine));
    mem = MemoryAllocator();
    scheduler = Scheduler();
    sem_manager = Semaphore();

    sem_manager.sem_open(&wait_for_char, 0);

    scheduler.idle_thread = scheduler.make_thread(idle_wrapper, nullptr, mem.mem_alloc(DEFAULT_STACK_SIZE));
    
    // gasimo spoljasnje hardverske prekide
    __asm__ volatile("csrw sie, %0" :: "r"((uint64)0b10));


    scheduler.thread_create(nullptr, cpp_test_wrapper, (void*)&userMain, mem.mem_alloc(DEFAULT_STACK_SIZE));

    scheduler.curr_active_thread = scheduler.pick_next();
    register_state = scheduler.curr_active_thread->context;
    interrupt_return();


}