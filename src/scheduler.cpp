#include "../h/syscalls.h"
#include "../h/scheduler.h"
#include "../h/memory_allocator.h"
#include "../h/syscall_c.h"

extern MemoryAllocator mem;
Scheduler scheduler;

Scheduler::Scheduler() {
    curr_active_thread = nullptr;
    idle_thread = nullptr;
    quantum_time_left = DEFAULT_TIME_SLICE;
    ready_head = nullptr;
    ready_tail = nullptr;
    sleeping_node = nullptr;
}

thread_t Scheduler::make_thread(void(*start_routine)(void*), void *arg, void *stack_space) {
    thread_t t = (thread_t)mem.mem_alloc(sizeof(_thread));
    if (t == nullptr) return nullptr;

    for (uint64 i = 0; i < sizeof(registers) / 8; i++) {
        // postavljamo sve registre na nulu
        ((uint64*)&t->context)[i] = 0;
    }

    void* stack_top = (void*)((uint64)stack_space + DEFAULT_STACK_SIZE);

    t->stack_head  = stack_space;
    t->start_routine = start_routine;
    t->arg = arg;

    // ~0xFULL -> ...11110000, unsigned long long
    t->context.sp = (uint64)stack_top & ~0xFULL; // sp mora biti deljiv sa 16

    // nit ne moze da sama pozove start_routine, mora da nakon sto se funkcija zavrsi, da izvrsi
    // poziv thread_exit. thread_wrapper automatski radi thread_exit.
    t->context.pc = (uint64)thread_wrapper;
    t->context.ra = (uint64)thread_wrapper;

    // a0 je registar za argumente. prvi (i jedini) argument iz funkcije thread_wrapper -> thread_t
    // self ce se procitati iz registra a0    
    t->context.a0 = (uint64)t;

    // gp i tp su isti za ceo program, zato samo kopiramo njihovu vrednost u kontekst nove niti.
    // citamo trenutnu vrednost registra gp i cuvamo je u t->context.gp (isto i za tp registar).
    __asm__ volatile("mv %0, gp" : "=r"(t->context.gp));
    __asm__ volatile("mv %0, tp" : "=r"(t->context.tp));

    return t;
}


int Scheduler::thread_create(thread_t *handle, void(*start_routine)(void*), void *arg, void *stack) {
    thread_t t = make_thread(start_routine, arg, stack);
    if (t == nullptr) return -1;

    put_ready(t);
    if (handle != nullptr) {
        *handle = t;
    }
    return 0;
}

void Scheduler::put_ready(thread_t t) {
    if (t == idle_thread) return;
    thread_ready_node *node = (thread_ready_node*)mem.mem_alloc(sizeof(thread_ready_node));
    node->thread = t;
    node->next = nullptr;

    // ukoliko je red prazan
    if (ready_tail == nullptr) {
        ready_head = ready_tail = node;
    } else {
        ready_tail->next = node;
        ready_tail = node;
    }
}

thread_t Scheduler::pick_next() {
    if (ready_head == nullptr) return idle_thread;
    
    // uzimamo head (FIFO)
    thread_ready_node *node = ready_head;
    thread_t t = node->thread;
    ready_head = node->next;
    if (ready_head == nullptr) ready_tail = nullptr;
    mem.mem_free(node);

    return t;
}

void thread_wrapper(thread_t self) {
    self->start_routine(self->arg);

    thread_exit();

    /* 
    explicit register variable
    https://gcc.gnu.org/onlinedocs/gcc/Explicit-Register-Variables.html
    
    promenljiva a0 mora uvek biti smestena u registar a0.
    zatim izvrsavamo ecall (sistemski rezim, interrupt_routine).
    scause = ENVIRONMENT_CALL_FROM_U_MODE
    a0 = 0x12
    */
    // register long a0 __asm__("a0") = NUM_THREAD_EXIT;
    // __asm__ volatile("ecall" : : "r" (a0));

    // nedostizno, ali ukoliko thread_exit ne pokrene context switch, procesor se vrti u petlji
    // while (true) {}
}

void Scheduler::put_sleep(thread_t t, time_t time) {
    thread_sleeping_node* node = (thread_sleeping_node*)mem.mem_alloc(sizeof(thread_sleeping_node));

    node->thread = t;
    node->period = time;
    node->next = sleeping_node;
    sleeping_node = node;
}

void Scheduler::tick_sleep() {
    thread_sleeping_node* curr = sleeping_node;
    thread_sleeping_node* prev = nullptr;       // -> za brisanje

    while (curr != nullptr) {
        curr->period--;

        if (curr->period == 0) {
            put_ready(curr->thread);
            thread_sleeping_node* to_delete = curr;

            if (prev == nullptr) {
                sleeping_node = curr->next;
            } else {
                prev->next = curr->next;
            }

            curr = curr->next;
            mem.mem_free(to_delete);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}