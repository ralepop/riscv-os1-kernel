#include "../h/interrupt_handler.h"
#include "../h/memory_allocator.h"
#include "../h/syscalls.h"
#include "../lib/hw.h"

extern MemoryAllocator mem;
extern "C" void interrupt_return();

extern "C" struct registers register_state = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

extern "C" uint64 kernel_stack;
uint64 kernel_stack;

// konzola
char putc_buffer[64];
char getc_buffer[64];
int buffer_size = 0;
static int free_element = 0;


extern "C" void interrupt_handler() {

    uint64 scause;
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));

    if (scause == ILLEGAL_INSTRUCTION || scause == LOAD_ACCESS_FAULT || scause == STORE_AMO_ACCESS_FAULT) {
        EXIT
    }

    // a0: kod sistemskog poziva, jednak broju iz prve kolone tabele date za C API

    uint64 syscall_code = register_state.a0;

    // ecall iz korisnickog rezima
    if (scause == ENVIRONMENT_CALL_FROM_U_MODE) {

    }
    
    // ecall iz sistemskog rezima
    if (scause == ENVIRONMENT_CALL_FROM_S_MODE) {
        
    }
    
    // spoljasnji hardverski prekid
    /* 
    Prekid od konzole je realizovan kao spoljašnji hardverski prekid. Postoji
    kontroler prekida preko kog se može dobiti informacija o tome koji uređaj je generisao prekid.
    Za to služi C funkcija plic_claim čija je deklaracija data u zaglavlju hw.h. Povratna
    vrednost ove funkcije je broj prekida.
    */
    if (scause == SUPERVISOR_EXTERNAL_INTERRUPT) {
        int interrupt_num = plic_claim();

        if (interrupt_num == 0x0a) {
            for (int i = 0; i < buffer_size; i++) {
                /* 
                U statusnom registru bit na poziciji 5 označava da kontroler konzole može primiti jedan podatak za slanje na konzolu (CONSOLE_TX_STATUS_BIT).
                Vrtimo se sve dok je taj bit nula. Kada = 1 znaci da je hardver spreman da primi novi bajt.
                */
                while (!(*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT));
                *(char*)CONSOLE_TX_DATA = putc_buffer[i];

                /* 
                U statusnom registru bit na poziciji 0 označava da se iz kontrolera konzole može pročitati podatak koji je stigao od konzole (CONSOLE_RX_STATUS_BIT).
                Vrtimo petlju sve dok je CONSOLE_RX_STATUS_BIT postavljen na 1 (zbog: U okviru jednog prekida mogu se prebacivati podaci sve dok su odgovarajući statusni biti na jedinici).
                */
                while (*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT) {
                    getc_buffer[free_element] = *(char*)CONSOLE_RX_DATA;
                    free_element = (free_element + 1) % 64;
                    /* TODO
                    ovde ide semafor da signalizira da ceka karakter:
                    sem_manager.sem_signal(wait_for_char);
                    */
                }
            }
        }

        plic_complete(interrupt_num);

        // https://docs.riscv.org/reference/isa/v20260120/priv/supervisor.html#11-1-1-3-supervisor-interrupt-sip-and-sie-registers
        // Gasimo spoljasnje hardverske prekide
        __asm__ volatile("csrw sie, %0" :: "r"(0b10));

    }

    // softverski prekid
    if (scause == SUPERVISOR_SOFTWARE_INTERRUPT) {

    }

}