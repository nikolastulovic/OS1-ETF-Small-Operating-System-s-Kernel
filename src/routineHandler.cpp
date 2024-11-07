#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Thread.hpp"
#include "../h/Semaphore.hpp"
#include "../lib/console.h"

extern "C" void routineHandler(){
    uint64 a7, a6, a5, a4, a3, a2, a1, a0;
    __asm__ volatile("sd a7, %0":"=m"(a7));
    __asm__ volatile("sd a6, %0":"=m"(a6));
    __asm__ volatile("sd a5, %0":"=m"(a5));
    __asm__ volatile("sd a4, %0":"=m"(a4));
    __asm__ volatile("sd a3, %0":"=m"(a3));
    __asm__ volatile("sd a2, %0":"=m"(a2));
    __asm__ volatile("sd a1, %0":"=m"(a1));
    __asm__ volatile("sd a0, %0":"=m"(a0));

    uint64 scause, sepc, sscratch;
    __asm__ volatile("csrr %0, sscratch":"=r"(sscratch));
    __asm__ volatile("csrr %0, scause":"=r"(scause));
    __asm__ volatile("csrr %0, sepc":"=r"(sepc));

    uint64 volatile sstatus;
    __asm__ volatile("csrr %0, sstatus":"=r"(sstatus));

    if(scause == 9UL || scause == 8UL){
        switch (a0) {
            case 0x01://mem_alloc
                a0=(uint64)MemoryAllocator::malloc(a1);
                break;
            case 0x02://mem_free
                a0=(uint64)MemoryAllocator::free((void*)a1);
                break;
            case 0x11://thread_create
                a0=(uint64)Kernel::Thread::createThread((Kernel::Thread**)a1,(void(*)(void*))a2,(void*)a3,(void*)a4);
                break;
            case 0x12://thread_exit
                a0=Kernel::Thread::running->threadExit();
                break;
            case 0x13://thread_dispatch
                Kernel::Thread::dispatch();
                break;
            case 0x14://thread_join
                Kernel::Thread::running->join((Kernel::Thread*)a1);
                break;
            case 0x21://sem_open
                a0=Kernel::Semaphore::SemaphoreOpen((Kernel::Semaphore**) a1,(unsigned)a2);
                break;
            case 0x22://sem_close
                a0=((Kernel::Semaphore*)a1)->SemaphoreClose();
                break;
            case 0x23://sem_wait
                a0=((Kernel::Semaphore*)a1)->wait();
                break;
            case 0x24://sem_signal
                a0=((Kernel::Semaphore*)a1)->signal();
                break;
            case 0x41://getc
                a0=(uint64)__getc();
                break;
            case 0x42://putc
                __putc((char)a1);
                break;
            default:
                break;
        }
        __asm__ volatile("sd a0,0x50(%0)"::"r"(sscratch));
        __asm__ volatile("csrw sepc, %0": :"r"(sepc+4));//IncrementPC
        __asm__ volatile("csrw sstatus, %0": :"r"(sstatus));
    }
    else if (scause == 0x8000000000000001UL){
        __asm__ volatile ("csrci sip, 0x02");
    }
    else if (scause == 0x8000000000000009UL){
        console_handler();
    }
}