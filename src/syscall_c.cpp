//
// Created by os on 5/23/23.
//
#include "../h/syscall_c.h"
#include "../lib/console.h"
char getc()
{
    __asm__ volatile("li a0, 0x41");

    __asm__ volatile("ecall");

    char ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
    //return __getc();
}

void putc(char chr)
{
    __asm__ volatile("mv a1, %0"::"r"(chr));
    __asm__ volatile("li a0, 0x42");

    __asm__ volatile("ecall");
    //__putc(chr);
}


void* mem_alloc(size_t size){
    size_t sz = size+1;
    if(size%MEM_BLOCK_SIZE==0)sz =size +MEM_BLOCK_SIZE;
    else{
        sz = sz + MEM_BLOCK_SIZE - (sz % MEM_BLOCK_SIZE);
    }
    sz = sz/MEM_BLOCK_SIZE;
    __asm__ volatile("mv a1, %0"::"r"(sz));
    __asm__ volatile("li a0, 0x01");

    __asm__ volatile("ecall");

    void* ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}

int mem_free(void *addr) {
    __asm__ volatile("mv a1, %0"::"r"(addr));
    __asm__ volatile("li a0, 0x02");

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}

int thread_create( thread_t* handle, void(*start_routine)(void*), void* arg)
{
    char* stack= (char*)mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile("mv a4, %0"::"r"(stack));
    __asm__ volatile("mv a3, %0"::"r"(arg));
    __asm__ volatile("mv a2, %0"::"r"(start_routine));
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("li a0, 0x11");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}
int thread_exit()
{
    __asm__ volatile("li a0, 0x12");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}
void thread_dispatch()
{
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile("ecall");
}
void thread_join(thread_t handle)
{
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("li a0, 0x14");
    __asm__ volatile("ecall");
}

int sem_open ( sem_t* handle, unsigned init )
{
    __asm__ volatile("mv a2, %0"::"r"(init));
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("li a0, 0x21");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}
int sem_close(sem_t handle)
{
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("li a0, 0x22");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}
int sem_wait(sem_t id)
{
    __asm__ volatile("mv a1, %0"::"r"(id));
    __asm__ volatile("li a0, 0x23");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}
int sem_signal(sem_t id)
{
    __asm__ volatile("mv a1, %0"::"r"(id));
    __asm__ volatile("li a0, 0x24");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0":"=r"(ret));
    return ret;
}