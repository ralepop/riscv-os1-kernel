#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include "../lib/hw.h"

void* mem_alloc(size_t size);           // 0x01
int mem_free (void*);                   // 0x02
size_t mem_get_free_space();            // 0x03
size_t mem_get_largest_free_block();    // 0x04


struct _thread;
typedef struct _thread* thread_t;

int thread_create(                      // 0x11
    thread_t* handle,
    void(*start_routine)(void*),
    void* arg
);

int thread_exit();                      // 0x12
void thread_dispatch();                 // 0x13


struct _sem;
typedef struct _sem* sem_t;

int sem_open(                           // 0x21
    sem_t* handle,
    unsigned init
);

int sem_close(sem_t handle);            // 0x22
int sem_wait(sem_t id);                 // 0x23
int sem_signal(sem_t id);               // 0x24
int sem_wait_n(sem_t id, unsigned n);   // 0x25
int sem_signal_n(sem_t id, unsigned n); // 0x26

typedef unsigned long time_t;
int time_sleep(time_t);                 // 0x31

const int EOF = -1;
char getc();                            // 0x41
void putc(char);                        // 0x42

#endif