// relja

#include "../h/semaphore.h"
#include "../h/memory_allocator.h"

extern MemoryAllocator mem;
Semaphore sem_manager;

Semaphore::Semaphore() {
    head = nullptr;
}

bool Semaphore::is_valid(sem_t id) {
    if (id == nullptr) {
        return false;
    }

    for (sem_node* node = head; node != nullptr; node = node->next) {
        if (node->sem == id) {
            return true;
        }
    }

    return false;
}

int Semaphore::sem_open(sem_t* handle, unsigned init) {
    if (handle == nullptr) {
        return -1;
    }

    sem_t s = (sem_t)mem.mem_alloc(sizeof(_sem));
    if (s == nullptr) {
        return -1;
    }

    s->num = init;
    s->thread_head = nullptr;
    s->thread_tail = nullptr;

    sem_node *node = (sem_node*)mem.mem_alloc(sizeof(sem_node));
    if (node == nullptr) {
        mem.mem_free(s);
        return -1;
    }

    node->sem = s;
    node->next = head;
    head = node;

    *handle = s;

    return 0;
}

int Semaphore::sem_close(sem_t handle) {
    if (!is_valid(handle)) {
        return -1;
    }

    // niti koje cekaju da se deblokiraju
    sem_wait_node* node = handle->thread_head;
    while (node != nullptr) {
        sem_wait_node* next = node->next;
        node->thread->context.a0 = (uint64)-1;
        scheduler.put_ready(node->thread);
        mem.mem_free(node);
        node = next;
    }

    // uklanjamo semafor iz liste otvorenih semafora
    sem_node* prev = nullptr;
    for (sem_node* t = head; t != nullptr; t = t->next) {
        if (t->sem == handle) {
            if (prev == nullptr) {
                head = t->next;
            } else {
                prev->next = t->next;
            }
            mem.mem_free(t);
            break;
        }
        prev = t;
    }

    mem.mem_free(handle);
    return 0;
}

// povratne vrednosti:
//  0 - uspeh, nit se nije blokirala
//  1 - nit je blokirana
// -1 - greska
int Semaphore::wait_impl(sem_t id, unsigned n) {
    if (!is_valid(id)) {
        return -1;
    }

    if (id->num >= n) {
        id->num -= n;
        return 0;
    }

    sem_wait_node* node = (sem_wait_node*)mem.mem_alloc(sizeof(sem_wait_node));
    node->thread = scheduler.curr_active_thread;
    node->n = n;
    node->next = nullptr;

    if (id->thread_tail == nullptr) {
        id->thread_head = id->thread_tail = node;
    } else {
        id->thread_tail->next = node;
        id->thread_tail = node;
    }

    return 1;
}

int Semaphore::signal_impl(sem_t id, unsigned n) {
    if (!is_valid(id)) {
        return -1;
    }

    id->num += n;

    // budimo FIFO sve koje mozemo
    while (id->thread_head != nullptr && id->thread_head->n <= id->num) {
        sem_wait_node* node = id->thread_head;
        id->thread_head = node->next;

        if (id->thread_head == nullptr) {
            id->thread_tail = nullptr;
        }

        id->num -= node->n;
        node->thread->context.a0 = 0;
        scheduler.put_ready(node->thread);

        mem.mem_free(node);
    }

    return 0;
}

int Semaphore::sem_wait(sem_t id) {
    return wait_impl(id, 1);
}

int Semaphore::sem_wait_n(sem_t id, unsigned n) {
    return wait_impl(id, n);
}

int Semaphore::sem_signal(sem_t id) {
    return signal_impl(id, 1);
}

int Semaphore::sem_signal_n(sem_t id, unsigned n) {
    return signal_impl(id, n);
}