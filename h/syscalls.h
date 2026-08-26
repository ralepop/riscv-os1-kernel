#ifndef SYSCALLS_H
#define SYSCALLS_H

#define NUM_MEM_ALLOC                   0x01
#define NUM_MEM_FREE                    0x02
#define NUM_MEM_GET_FREE_SPACE          0x03
#define NUM_MEM_GET_LARGEST_FREE_BLOCK  0x04

#define NUM_THREAD_CREATE               0x11
#define NUM_THREAD_EXIT                 0x12
#define NUM_THREAD_DISPATCH             0x13

#define NUM_SEM_OPEN                    0x21
#define NUM_SEM_CLOSE                   0x22
#define NUM_SEM_WAIT                    0x23
#define NUM_SEM_SIGNAL                  0x24
#define NUM_SEM_WAIT_N                  0x25
#define NUM_SEM_SIGNAL_N                0x26

#define NUM_TIME_SLEEP                  0x31

#define NUM_GETC                        0x41
#define NUM_PUTC                        0x42

// https://docs.riscv.org/reference/isa/v20260120/priv/supervisor.html#scause

// MSB (BNT) = 1
#define SUPERVISOR_SOFTWARE_INTERRUPT   0x8000000000000001 // softverski prekid
#define SUPERVISOR_EXTERNAL_INTERRUPT   0x8000000000000009 // spoljasnji hardverski prekid

// MSB (BNT) = 0
#define ILLEGAL_INSTRUCTION             0x2 // ilegalna instrukcija
#define LOAD_ACCESS_FAULT               0x5 // nedozvoljena adresa citanja
#define STORE_AMO_ACCESS_FAULT          0x7 // nedozvoljena adresa upisa
#define ENVIRONMENT_CALL_FROM_U_MODE    0x8 // ecall iz korisnickog rezima
#define ENVIRONMENT_CALL_FROM_S_MODE    0x9 // ecall iz sistemskog rezima

#endif