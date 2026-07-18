#include "../h/MemoryAllocator.h"
#include "../lib/console.h"

extern "C" void interrupt() {

}

extern MemoryAllocator mem;
MemoryAllocator mem;

void print_hex(uint64 n) {
    char hex_chars[] = "0123456789abcdef";
    char buffer[16];
    int i = 0;

    if (n == 0) {
        __putc('0');
        return;
    }

    while (n > 0) {
        buffer[i++] = hex_chars[n % 16];
        n /= 16;
    }

    while (i > 0) {
        __putc(buffer[--i]);
    }

}

void print_list() {
    __putc('\n');
    __putc('H'); __putc('E'); __putc('A'); __putc('P'); __putc(':'); __putc(' ');

    Fragment *curr = mem.head;
    while (curr != nullptr) {
        __putc('[');
        print_hex((uint64)curr);
        __putc('|');
        print_hex(curr->number_of_blocks);
        __putc(']');

        if (curr->next != nullptr) {
            uint64 curr_end = (uint64)curr + (curr->number_of_blocks * MEM_BLOCK_SIZE);
            if ((uint64)curr->next < curr_end) {
                __putc(' '); __putc('!'); __putc('P'); __putc('R'); __putc('E');
                __putc('K'); __putc('L'); __putc('O'); __putc('P'); __putc('!');
            } else {
                __putc('-'); __putc('>');
            }
        }
        curr = curr->next;
    }
    __putc('n'); __putc('u'); __putc('l'); __putc('l');
    __putc('\n');
}

void run_allocator_test() {
    __putc('T'); __putc('E'); __putc('S'); __putc('T'); __putc('\n'); 

    void *p1 = mem.mem_alloc(64); // ima deljenje fragmenata
    void *p2 = mem.mem_alloc(67097696);
    void *p3 = mem.mem_alloc(64);

    print_list();

    if (p1 == nullptr || p2 == nullptr || p3 == nullptr) {
        __putc('E'); __putc('R'); __putc('R'); __putc('O'); __putc('R'); __putc('\n');
        return;
    }

    mem.mem_free(p2);
    print_list();
    mem.mem_free(p1);
    print_list();
    mem.mem_free(p3);
    print_list();

    __putc('O'); __putc('K'); __putc('\n'); 

}

void main() {

    asm volatile("csrw stvec,%0"::"r"(interrupt));
    mem = MemoryAllocator();

    run_allocator_test();
}