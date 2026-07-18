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
    __putc('C'); __putc('O'); __putc('A'); __putc('L'); __putc('E'); __putc('S'); __putc('C'); __putc('E'); __putc('\n');

    void *p1 = mem.mem_alloc(64);
    void *p2 = mem.mem_alloc(64);
    void *p3 = mem.mem_alloc(64);
    void *p4 = mem.mem_alloc(64);


    mem.mem_free(p2);
    mem.mem_free(p3);
    __putc('1'); __putc(' ');
    print_list();

    mem.mem_free(p1);
    __putc('2'); __putc(' ');
    print_list();

    mem.mem_free(p4);
    __putc('3'); __putc(' ');
    print_list();
}

void main() {

    asm volatile("csrw stvec,%0"::"r"(interrupt));
    mem = MemoryAllocator();

    run_allocator_test();
}