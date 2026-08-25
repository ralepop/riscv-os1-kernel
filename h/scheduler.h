#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../lib/hw.h"

#define EXIT *(uint32*)0x100000 = 0x5555;

struct registers {
    uint64 ra;
    uint64 sp;
    uint64 s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint64 a0, a1, a2, a3, a4, a5, a6, a7;
    uint64 t0, t1, t2, t3, t4, t5, t6;
    uint64 gp;
    uint64 tp;
    uint64 pc;
};

// konzola
extern char putc_buffer[64];
extern char getc_buffer[64];
extern int buffer_size;

struct _thread {
    registers context;
    void *stack_head;
    void (*start_routine)(void*);
    void *arg;
};

typedef struct _thread *thread_t;

struct thread_ready_node {
    thread_t thread;
    thread_ready_node *next;
};

void thread_wrapper(thread_t self);

struct thread_sleeping_node {
    time_t period;
    thread_t thread;
    thread_sleeping_node *next;
};

// round-robin algoritam
class Scheduler {
public:
    Scheduler();

    thread_t curr_active_thread;
    thread_t idle_thread;
    long int quantum_time_left;

    thread_ready_node *ready_head;
    thread_ready_node *ready_tail;
    thread_sleeping_node *sleeping_node;

    thread_t make_thread(void(*start_routine)(void*), void *arg, void *stack_space);
    int thread_create(thread_t *handle, void(*start_routine)(void*), void *arg, void *stack);
    void put_ready(thread_t t);
    thread_t pick_next();

    // int thread_exit();
    // int time_sleep(time_t period);
};

extern Scheduler scheduler;

#endif