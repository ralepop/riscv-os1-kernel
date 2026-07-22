#ifndef INTERRUPT_ROUTINE_H
#define INTERRUPT_ROUTINE_H

#include "../lib/hw.h"

extern "C" {
    void interrupt_handler();
}

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

#endif