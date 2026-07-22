#include "../lib/console.h"
#include "../lib/hw.h"

struct registers {
    uint64 ra;
    uint64 sp;
    uint64 s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint64 a0, a1, a2, a3, a4, a5, a6, a7;
    uint64 t0, t1, t2, t3, t4, t5, t6;
    uint64 gp;
    uint64 tp;
    uint64 pc;
} registers;

struct registers register_state = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


extern "C" void interrupt_routine();

extern "C" void interrupt_handler() {
    uint64 scause;
    __asm__ volatile("csrr %0, scause" : "=r"(scause));
    register_state.pc += 4;
}

alignas(16) static char kstack_space[DEFAULT_STACK_SIZE];
extern "C" uint64 kernel_stack = (uint64)(kstack_space + DEFAULT_STACK_SIZE);

void main() {

    uint64 volatile saved_stvec = (uint64)&interrupt_routine;
    __asm__ volatile("csrw stvec,%0"::"r"(saved_stvec));

    __asm__ volatile("ecall");
    __putc('e'); __putc('c'); __putc('a'); __putc('l');  __putc('l');

}