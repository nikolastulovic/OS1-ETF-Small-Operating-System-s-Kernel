//
// Created by os on 5/27/23.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_SEMAPHORE_HPP
#include "Thread.hpp"
#include "MemoryAllocator.hpp"
namespace Kernel
{
    class Semaphore {
    public:
        Semaphore (unsigned init = 1):val(init) { }
        int wait ();
        int signal ();
        static int SemaphoreOpen(Semaphore** handle,unsigned init);
        int SemaphoreClose();
        int value()const {return val;}
    protected:
        void block();
        void unblock();
    private:
        int val;
        //List<Thread> blocked;
        struct Lista{
            Thread* t;
            Lista* next;
            Lista(Thread*t,Lista*next):t(t),next(next){ }
        };
        Lista *head=nullptr,*tail=nullptr;
    };
}


#endif //PROJECT_BASE_V1_1_SEMAPHORE_HPP
