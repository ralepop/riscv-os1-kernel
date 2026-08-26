#include "../h/syscall_cpp.hpp"

void* operator new(size_t size) {
    void* ptr = mem_alloc(size);
    return ptr;
}

void operator delete(void* ptr) {
    mem_free(ptr);
}

Thread::Thread(void (*body)(void*), void* arg) : myHandle(nullptr), body(body), arg(arg) {}

Thread::Thread() : myHandle(nullptr), body(nullptr), arg(nullptr) {}

Thread::~Thread() {}

int Thread::start() {
    return thread_create(&myHandle, &Thread::wrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::wrapper(void* arg) {
    Thread* self = (Thread*)arg;

    if (self->body != nullptr) {
        self->body(self->arg);
    } else {
        self->run();
    }
}

Semaphore::Semaphore(unsigned init) : myHandle(nullptr) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

PeriodicThread::PeriodicThread(time_t period) : Thread(), period(period) {}

void PeriodicThread::terminate() {
    period = 0;
}

void PeriodicThread::run() {

    // kada je period == 0 znaci da je nit zaustavljena
    while (period != 0) {
        periodicActivation();

        if (period != 0) {
            sleep(period);
        }
    }
}

char Console::getc() {
    // pozivamo C funkciju definisanu u syscall_c.h
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}