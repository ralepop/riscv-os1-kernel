# 1 "src/interrupt_return.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/interrupt_return.S"
.globl interrupt_return
.globl interrupt_routine
.globl register_state
.globl kernel_stack

interrupt_return:
    la ra, kernel_stack;
    sd sp, (ra);
    la ra, register_state;

    ld a0, 248(ra);
    csrw sepc, a0;

    ld sp, 8(ra);
    ld s0, 16(ra);
    ld s1, 24(ra);
    ld s2, 32(ra);
    ld s3, 40(ra);
    ld s4, 48(ra);
    ld s5, 56(ra);
    ld s6, 64(ra);
    ld s7, 72(ra);
    ld s8, 80(ra);
    ld s9, 88(ra);
    ld s10, 96(ra);
    ld s11, 104(ra);
    ld a0, 112(ra);
    ld a1, 120(ra);
    ld a2, 128(ra);
    ld a3, 136(ra);
    ld a4, 144(ra);
    ld a5, 152(ra);
    ld a6, 160(ra);
    ld a7, 168(ra);
    ld t0, 176(ra);
    ld t1, 184(ra);
    ld t2, 192(ra);
    ld t3, 200(ra);
    ld t4, 208(ra);
    ld t5, 216(ra);
    ld t6, 224(ra);
    ld gp, 232(ra);
    ld tp, 240(ra);
    ld ra, (ra);
    sret;
