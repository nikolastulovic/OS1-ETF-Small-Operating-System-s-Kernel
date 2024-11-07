//
// Created by os on 5/26/23.
//

#ifndef PROJECT_BASE_V1_1_THREAD_HPP
#define PROJECT_BASE_V1_1_THREAD_HPP
#include "../lib/hw.h"
#include "MemoryAllocator.hpp"

namespace Kernel{

    void popSppSpie();

    class Thread;
    class Semaphore;
    class Scheduler {
    private:
        struct Lista{
            Thread* t;
            Lista* next;
            Lista(Thread*t,Lista*next):t(t),next(next){ }
        };
        static Lista *head,*tail;

        //static List<Thread> readyQueue;
        static Scheduler *instance;
    public:
        static Scheduler *init();

        static Thread *get();

        static void put(Thread *t);
    };

    class Thread {
    public:
        bool isFinished() const { return finished; }
        void setFinished(bool finished) {this->finished=finished;}
        bool isBlocked() const { return blocked; }
        void setBlocked(bool blocked) {this->blocked=blocked;}
        void join(Thread* handle);
        static void yield();
        using Body = void (*)(void*);
        static Thread* running;
        static int createThread(Thread** handle, Body body, void* arg, void* stack);
        static int threadExit();
        static void dispatch();
    protected:
        struct Context{
            uint64 ra;
            uint64 sp;
        };
        Context context;

        static void contextSwitch(Context* oldContext, Context* runningContext);
        friend class Kernel::Semaphore ;
    private:

        static void wrapper();
        Body body;//void (*body)(void*);
        void* arg;
        char* stack;
        bool finished;
        bool blocked;
        bool started;
    };
}


#endif //PROJECT_BASE_V1_1_THREAD_HPP
