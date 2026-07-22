#include "../h/interrupt_routine.h"
#include "../lib/console.h"
#include "../lib/hw.h"

extern "C" struct registers registers;

void interrupt_handler() {
    uint64 scause;
    __asm__ volatile("csrr %0, scause" : "=r"(scause));
    registers.pc += 4;

}