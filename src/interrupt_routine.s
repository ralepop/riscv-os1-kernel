# 1 "src/interrupt_routine.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/interrupt_routine.S"
.globl interrupt_routine
.globl register_state
.globl kernel_stack

interrupt_routine:
    csrw sscratch, ra;
    la ra, register_state;
    sd sp, 8(ra);
    sd s0, 16(ra);
    sd s1, 24(ra);
    sd s2, 32(ra);
    sd s3, 40(ra);
    sd s4, 48(ra);
    sd s5, 56(ra);
    sd s6, 64(ra);
    sd s7, 72(ra);
    sd s8, 80(ra);
    sd s9, 88(ra);
    sd s10, 96(ra);
    sd s11, 104(ra);
    sd a0, 112(ra);
    sd a1, 120(ra);
    sd a2, 128(ra);
    sd a3, 136(ra);
    sd a4, 144(ra);
    sd a5, 152(ra);
    sd a6, 160(ra);
    sd a7, 168(ra);
    sd t0, 176(ra);
    sd t1, 184(ra);
    sd t2, 192(ra);
    sd t3, 200(ra);
    sd t4, 208(ra);
    sd t5, 216(ra);
    sd t6, 224(ra);
    sd gp, 232(ra);
    sd tp, 240(ra);
    mv a0, ra;
    csrrw ra, sscratch, ra;
    sd ra, (a0);
    csrr t0, sepc;
    sd t0, 248(a0);
    la ra, kernel_stack;
    ld sp, (ra);
    call interrupt_handler;
    call interrupt_return;
