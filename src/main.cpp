#include "../lib/console.h"
#include "../lib/hw.h"

void main() {

    __asm__ volatile("csrw stvec,%0"::"r"());

}