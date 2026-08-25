#include "../h/scheduler.h"
#include "../h/syscalls.h"

void thread_wrapper(thread_t self) {
    self->start_routine(self->arg);

    /* 
    explicit register variable
    https://gcc.gnu.org/onlinedocs/gcc/Explicit-Register-Variables.html
    
    promenljiva a0 mora uvek biti smestena u registar a0.
    zatim izvrsavamo ecall (sistemski rezim, interrupt_routine).
    scause = ENVIRONMENT_CALL_FROM_U_MODE
    a0 = 0x12
    */
    register long a0 __asm__("a0") = NUM_THREAD_EXIT;
    __asm__ volatile("ecall");

    // nedostizno, ali ukoliko thread_exit ne pokrene context switch procesor se vrti u petlji
    while (true) {}
}