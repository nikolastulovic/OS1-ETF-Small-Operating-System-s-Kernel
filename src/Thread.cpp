#include "../h/Thread.hpp"
#include "../h/syscall_c.h"
extern  "C" void pushRegisters();
extern  "C" void popRegisters();

Kernel::Thread* Kernel::Thread::running=nullptr;
Kernel::Scheduler *Kernel::Scheduler::instance=nullptr;
Kernel::Scheduler::Lista *Kernel::Scheduler::head=nullptr;
Kernel::Scheduler::Lista *Kernel::Scheduler::tail=nullptr;

//Thread
void Kernel::popSppSpie() {
    __asm__ volatile("csrw sepc,ra");
    __asm__ volatile("csrc sstatus, %0": : "r"((uint64)256));
    __asm__ volatile("sret");
}

void Kernel::Thread::wrapper() {

    Kernel::popSppSpie();
    running->body(running->arg);
    //kraj, brisanje niti
    running->setFinished(true);

    if (running->stack != nullptr) {
        MemoryAllocator::free(running->stack);
        running->stack = nullptr;
    }

    thread_dispatch();
}

int Kernel::Thread::createThread(Thread** handle, Thread::Body body, void* arg, void* stack)
{
    size_t argSize=sizeof(Thread)/MEM_BLOCK_SIZE+(sizeof(Thread)%MEM_BLOCK_SIZE==0?0:1);
    Thread* elem=(Thread*)MemoryAllocator::malloc(argSize);
    if(!elem) return -1;
    if(body==nullptr)
    {
        elem->body=nullptr;
        elem->arg=nullptr;
        elem->stack=nullptr;
        elem->context.ra=0;
        elem->context.sp=0;
        elem->finished=false;
    }
    else
    {
        elem->body=body;
        elem->arg=arg;
        elem->stack=(char*)stack;
        elem->context.ra = (uint64)&Thread::wrapper;
        elem->context.sp=(uint64)&elem->stack[DEFAULT_STACK_SIZE];
        elem->finished=false;
    }
    elem->blocked=false;
    *handle=elem;
    elem->started=true;
    if(body!=nullptr) Scheduler::put(elem);
    return 0;
}

void Kernel::Thread::yield() {
    pushRegisters();
    Thread::dispatch();
    popRegisters();
}

void Kernel::Thread::dispatch() {
    Thread*old=running;
    if(!old->isFinished() && !old->blocked){ Scheduler::put(old); }

    running=Scheduler::get();
    Thread::contextSwitch(&old->context, &running->context);
}

void Kernel::Thread::join(Thread *handle)
{
    while(!handle->isFinished())
    {
        dispatch();
    }
}

int Kernel::Thread::threadExit() {
    if(!running) return -1;//negativna vrednost ako nesto nije u redu

    running->setFinished(true);

    if (running->stack != nullptr) {
        MemoryAllocator::free(running->stack);
        running->stack = nullptr;
    }

    dispatch();

    return 0;
}

//Scheduler
Kernel::Scheduler* Kernel::Scheduler::init()
{
    if(!instance)
    {
        size_t argSize=sizeof(Scheduler)/MEM_BLOCK_SIZE+(sizeof(Scheduler)%MEM_BLOCK_SIZE==0?0:1);
        Scheduler* elem=(Scheduler*)MemoryAllocator::malloc(argSize);
        instance=elem;
    }
    return instance;
}

Kernel::Thread *Kernel::Scheduler::get()
{
    //return readyQueue.removeFirst();
    if(!head) return nullptr;
    Lista*elem=head;
    head=head->next;
    if(!head) tail=nullptr;

    Thread* ret=elem->t;
    MemoryAllocator::free(elem);
    return ret;
}

void Kernel::Scheduler::put(Thread* t)
{
    //readyQueue.addLast(t);
    size_t argSize=sizeof(Lista)/MEM_BLOCK_SIZE+(sizeof(Lista)%MEM_BLOCK_SIZE==0?0:1);
    Lista* elem=(Lista*)MemoryAllocator::malloc(argSize);
    elem->t=t;
    elem->next=nullptr;

    if(tail){
        tail->next=elem;
        tail=elem;
    }
    else head=tail=elem;
}