#include "../h/MemoryAllocator.h"


MemoryAllocator::MemoryAllocator() {
    
    head = (Fragment*)(HEAP_START_ADDR);
    
    /* 
     * Radi se provera da li je adresa head poravnata na 64 bajta.
     * Poslednjih 6 bitova treba da budu 0 (da bi bili deljivi sa 64),
     * ukoliko je bilo koji postavljen na 1 onda nije poravnato.
     */
    if ((uint64)head & 0b111111) {

        /* 
         * Pomeranje ima dva koraka:
         * 1 - sabiranje sa 64 (0b1000000)
         * 2 - round down na sledeci najblizi broj koji je deljiv sa 64
         *
         * 0xFFFFFFFFFFFFFFC0 = 0b....000000 (poslednje 6 su nule)
         */
        head = (Fragment*)(((uint64)head + 0b1000000) & 0xFFFFFFFFFFFFFFC0);
    }

    head->number_of_blocks = ((uint64)HEAP_END_ADDR - (uint64)head) / MEM_BLOCK_SIZE;
    head->next = nullptr;
    head->prev = nullptr;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    
    // Moramo da uvecamo jer cuvamo next, prev i num_blocks.
    size += sizeof(Fragment);
    uint64 blocks = size / MEM_BLOCK_SIZE;

    if (size % MEM_BLOCK_SIZE != 0) {
        blocks++;
    }

    // First fit algoritam
    Fragment *curr = head;

    while (curr != nullptr) {
        // Staje
        if (curr->number_of_blocks >= blocks) {
            // Staje tacno
            if (curr->number_of_blocks == blocks) {
                if (curr == head) {
                    head = curr->next;
                    if (head != nullptr) {
                        head->prev = nullptr;
                    }
                } else {
                    curr->prev->next = curr->next;
                    if (curr->next != nullptr) {
                        curr->next->prev = curr->prev;
                    }
                }
            } 
            // Delimo fragmente
            else {
                // Na pocetnu adresu (curr) dodajemo broj bajtova (blocks * MEM_BLOCK_SIZE)
                Fragment *f = (Fragment*)(blocks * MEM_BLOCK_SIZE + (uint64)curr);
                
                f->prev = curr->prev;
                f->next = curr->next;
                
                if (f->next != nullptr) {
                    f->next->prev = f;
                }

                if (curr != head) {
                    f->prev->next = f;
                } else {
                    head = f;
                }

                f->number_of_blocks = curr->number_of_blocks - blocks;
                curr->number_of_blocks = blocks;
            }

            return (void*)(curr + 1);
        }

        curr = curr->next;
    }

    return nullptr;
}

int MemoryAllocator::mem_free(void *ptr) {

    Fragment *hptr = (Fragment*) ptr - 1;

    // A -> B -> C -> D
    //      .

    Fragment *curr = head;
    
    // ukoliko je curr == nullptr, curr->prev baca seg. fault, te koristimo pomocni pokazivac
    Fragment *prev = nullptr;

    while (curr != nullptr && hptr > curr) {
        prev = curr;
        curr = curr->next;
    }

    hptr->next = curr;
    hptr->prev = prev;

    if (prev != nullptr) {
        prev->next = hptr;
    } else {
        head = hptr;
    }

    if (curr != nullptr) {
        curr->prev = hptr;
    }

    /* coalescing */ 

    // Mergujemo sledeci (desno), ako je moguce
    if (hptr->next != nullptr) {

        /*
            [ HPTR FRAGMENT ]  [ NEXT FRAGMENT ]
            ^                  ^
            |                  |
            hptr            hptr->next
                            next_end
        */

        char *next_end = (char*)hptr + hptr->number_of_blocks * MEM_BLOCK_SIZE;

        if ((Fragment*)next_end == hptr->next) {

            hptr->number_of_blocks += hptr->next->number_of_blocks;
            hptr->next = hptr->next->next;

            if (hptr->next != nullptr) {
                hptr->next->prev = hptr;
            }
        }
    }

    // Mergujemo prethodni (levo), ako je moguce
    if (hptr->prev != nullptr) {

        /* 
            [ PREV FRAGMENT ]  [ HPTR FRAGMENT ]
                               ^
                               |
                            prev_end (kraj)
                              hptr
        */

        char *prev_end = (char*)hptr->prev + (hptr->prev->number_of_blocks * MEM_BLOCK_SIZE);

        if ((Fragment*)prev_end == hptr) {

            hptr->prev->number_of_blocks += hptr->number_of_blocks;
            hptr->prev->next = hptr->next;

            if (hptr->next != nullptr) {
                hptr->next->prev = hptr->prev;
            }

            hptr = hptr->prev;
        }
    }

    return 0;
}