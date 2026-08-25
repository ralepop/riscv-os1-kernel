#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/memory_allocator.h"
#include "../h/scheduler.h"

extern "C" void userMain();

// user_main_body ima odgovarajuci potpis - void(*)(void*)
void user_main_body(void*) { userMain(); }

// besposlena nit
void idle_body(void*) { while (true) {} }

MemoryAllocator mem;
extern "C" void interrupt_routine();
extern "C" void interrupt_return();
extern "C" struct registers register_state;


void main() {

    // kada se desi prekid izvrsavamo kod cija se adresa nalazi u funkciji interrupt_routine
    __asm__ volatile("csrw stvec, %0" :: "r"(interrupt_routine));
    mem = MemoryAllocator();
    
    // idle nit, nikada ne ide u ready red (make_thread)
    void *idle_stack = mem.mem_alloc(DEFAULT_STACK_SIZE);
    scheduler.idle_thread = scheduler.make_thread(
        idle_body,
        nullptr,
        (void*)((uint64)idle_stack + DEFAULT_STACK_SIZE - 1) // racunamo vrh steka
    );

    // glavna nit, ubacujemo je u red spremnih (thread_create)
    thread_t main_thread;
    void *main_stack = mem.mem_alloc(DEFAULT_STACK_SIZE);
    scheduler.thread_create(
        &main_thread,
        user_main_body, // userMain()
        nullptr,
        (void*)((uint64)main_stack + DEFAULT_STACK_SIZE - 1)
    );

    scheduler.curr_active_thread = scheduler.pick_next();
    register_state = scheduler.curr_active_thread->context;

    uint64 sstatus;
    __asm__ volatile("csrr %0, sstatus");

    // gasimo spoljasnje hardverske prekide
    __asm__ volatile("csrw sie, %0" :: "r"((uint64)0b10));
}