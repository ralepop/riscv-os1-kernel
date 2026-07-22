#include "../lib/console.h"
#include "../lib/hw.h"

extern "C" void interrupt_routine();


void main() {

    uint64 volatile saved_stvec = (uint64)&interrupt_routine;

    __asm__ volatile("csrw stvec,%0"::"r"(saved_stvec));

}