//
// Created by os on 5/27/23.
//

#include "../h/Semaphore.hpp"

void Kernel::Semaphore :: block () {

    size_t argSize=sizeof(Lista)/MEM_BLOCK_SIZE+(sizeof(Lista)%MEM_BLOCK_SIZE==0?0:1);
    Lista* elem=(Lista*)MemoryAllocator::malloc(argSize);

    elem->t=Thread :: running;
    elem->next=nullptr;

    //stavljanje u listu blokiranih  blocked.addLast(Thread :: running);
    if(tail){
        tail->next=elem;
        tail=elem;
    }
    else head=tail=elem;

    Thread::running->setBlocked(true);

    Thread::dispatch();
}
void Kernel::Semaphore::unblock () {
    //Thread* t = blocked.removeFirst();
    if(!head) return;
    Lista* elem=head;
    head=head->next;
    if(!head) tail=0;

    Thread* t =elem->t;
    MemoryAllocator::free(elem);

    t->setBlocked(false);
    Scheduler :: put(t);
}
int Kernel::Semaphore::wait () {
    if (--val < 0)
    {
        block();
    }
    return 0;
}
int Kernel::Semaphore::signal () {
    if (++val <= 0)
    {
        unblock();
    }
    return 0;
}
int Kernel::Semaphore::SemaphoreOpen(Kernel::Semaphore** handle,unsigned init)
{
    size_t argSize=sizeof(Semaphore)/MEM_BLOCK_SIZE+(sizeof(Semaphore)%MEM_BLOCK_SIZE==0?0:1);
    Semaphore* elem=(Semaphore*)MemoryAllocator::malloc(argSize);
    if(!elem) return -1;

    elem->head= nullptr;
    elem->tail= elem->head;
    elem->val=init;
    *handle=elem;

    return 0;
}

int Kernel::Semaphore::SemaphoreClose()
{
    if(!head) return 0;
    Lista* elem=head;
    head=head->next;
    if(!head) tail=0;

    Thread* cur =elem->t;
    MemoryAllocator::free( elem);
    while(cur)
    {
        Scheduler :: put(cur);
        if(!head) break;
        Lista* elem=head;
        head=head->next;
        if(!head) tail=0;

        cur =elem->t;
        MemoryAllocator::free( elem);
    }
    return 0;
    /*Thread* cur;
    while((cur=blocked.removeFirst()))
    {
        Scheduler :: put(cur);
    }
    handles.removeData(this);*/
}