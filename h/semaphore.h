#ifndef SEMAPHORE_H
#define SEMAPHORE_H

extern "C" {
    #include "syscall_c.h"
}

#include "scheduler.h"

struct sem_wait_node {
    thread_t thread;
    unsigned n;             // koliko jedinica nit ceka
    sem_wait_node* next;
};

struct _sem {
    unsigned num;
    sem_wait_node* thread_head;
    sem_wait_node* thread_tail;
};

struct sem_node {
    sem_t sem;
    sem_node* next;
};

class Semaphore {
public:
    sem_node* head;
    Semaphore();

    int sem_open(sem_t* handle, unsigned init);
    int sem_close(sem_t handle);
    int sem_wait(sem_t id);
    int sem_signal(sem_t id);
    int sem_wait_n(sem_t id, unsigned n);
    int sem_signal_n(sem_t id, unsigned n);

private:
    bool is_valid(sem_t id);
    int wait_impl(sem_t id, unsigned n);
    int signal_impl(sem_t id, unsigned n);
};

extern Semaphore sem_manager;

#endif