//
// Created by os on 5/23/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SYSCALL_C_H
#include "../lib/hw.h"
#include "Thread.hpp"
#include "Semaphore.hpp"

void*mem_alloc(size_t);
int mem_free(void* addr);

typedef Kernel::Thread* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t handle);

typedef Kernel::Semaphore* sem_t;
int sem_open (
        sem_t* handle,
        unsigned init
);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = 1;
void putc(char chr);
char getc();
#endif //PROJECT_BASE_V1_1_SYSCALL_C_H
