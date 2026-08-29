#include "../h/syscall_c.h"
#include "../h/syscalls.h"

extern sem_t wait_for_char;
const int EOF = -1;

void* mem_alloc(size_t size) {

    size_t blocks = size / MEM_BLOCK_SIZE;
    if (size % MEM_BLOCK_SIZE != 0) blocks++;

    register long a0 __asm__("a0") = NUM_MEM_ALLOC;
    register long a1 __asm__("a1") = (long)blocks;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
    return (void*)a0;
}

int mem_free (void* adr) {
    register long a0 __asm__("a0") = NUM_MEM_FREE;
    register long a1 __asm__("a1") = (long)adr;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
    return (int)a0;
}

size_t mem_get_free_space() {
    register long a0 __asm__("a0") = NUM_MEM_GET_FREE_SPACE;
    __asm__ volatile("ecall" : "+r"(a0));
    return (size_t)a0;
}

size_t mem_get_largest_free_block() {
    register long a0 __asm__("a0") = NUM_MEM_GET_LARGEST_FREE_BLOCK;
    __asm__ volatile("ecall" : "+r"(a0));
    return (size_t)a0;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    if (stack == nullptr) {
        return -1;
    }

    register long a0 __asm__("a0") = NUM_THREAD_CREATE;
    register long a1 __asm__("a1") = (long)handle;
    register long a2 __asm__("a2") = (long)start_routine;
    register long a3 __asm__("a3") = (long)arg;
    register long a4 __asm__("a4") = (long)stack;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a3), "r"(a4));
    return (int)a0;
}

int thread_exit() {
    register long a0 __asm__("a0") = NUM_THREAD_EXIT;
    __asm__ volatile("ecall" : "+r"(a0));
    return (int)a0;
}

void thread_dispatch() {
    register long a0 __asm__("a0") = NUM_THREAD_DISPATCH;
    __asm__ volatile("ecall" : "+r"(a0));
}

int sem_open(sem_t *handle, unsigned int init) {
    register long a0 __asm__("a0") = NUM_SEM_OPEN;
    register long a1 __asm__("a1") = (long)handle;
    register long a2 __asm__("a2") = (long)init;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2));
    return (int)a0;
}

int sem_close(sem_t handle) {
    register long a0 __asm__("a0") = NUM_SEM_CLOSE;
    register long a1 __asm__("a1") = (long)handle;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
    return (int)a0;
}

int sem_wait(sem_t id) {
    register long a0 __asm__("a0") = NUM_SEM_WAIT;
    register long a1 __asm__("a1") = (long)id;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
    return (int)a0;
}

int sem_signal(sem_t id) {
    register long a0 __asm__("a0") = NUM_SEM_SIGNAL;
    register long a1 __asm__("a1") = (long)id;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
    return (int)a0;
}

int sem_wait_n(sem_t id, unsigned n) {
    register long a0 __asm__("a0") = NUM_SEM_WAIT_N;
    register long a1 __asm__("a1") = (long)id;
    register long a2 __asm__("a2") = (long)n;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2));
    return (int)a0;
}

int sem_signal_n(sem_t id, unsigned n) {
    register long a0 __asm__("a0") = NUM_SEM_SIGNAL_N;
    register long a1 __asm__("a1") = (long)id;
    register long a2 __asm__("a2") = (long)n;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2));
    return (int)a0;
}

int time_sleep(time_t time) {
    if (time == 0) {
        return -1;
    }

    register long a0 __asm__("a0") = NUM_TIME_SLEEP;
    register long a1 __asm__("a1") = (long)time;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
    return (int)a0;
}

char getc() {
    sem_wait(wait_for_char);
    register long a0 __asm__("a0") = NUM_GETC;
    __asm__ volatile("ecall" : "+r"(a0));
    return (char)a0;
}

void putc(char c) {
    register long a0 __asm__("a0") = NUM_PUTC;
    register long a1 __asm__("a1") = (long)c;
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a1));
}