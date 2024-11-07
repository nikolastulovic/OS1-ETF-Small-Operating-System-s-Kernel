#include "../h/syscall_cpp.hpp"

//memorija
void* operator new (size_t sz)
{
    return mem_alloc(sz);
}
void operator delete (void* addr)
{
    mem_free(addr);
}

//Thread

Thread::Thread(void (*body)(void *), void *arg) :body(body),arg(arg){ }

int Thread::start() {
    if(body) return thread_create(&myHandle,body,arg);
    auto wrapper=[](void* ths){
        ((Thread*)ths)->run();
    };
    return thread_create(&myHandle,wrapper,this);
}
void Thread::join()
{
    thread_join(myHandle);
}
void Thread::dispatch ()
{
    thread_dispatch();
}

Thread::~Thread ()
{
    thread_join(myHandle);
}
//Semafor

Semaphore::Semaphore (unsigned init)
{
    sem_open( &myHandle, init );
}
Semaphore::~Semaphore ()
{
    sem_close(myHandle);
}
int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal ()
{
    return sem_signal(myHandle);
}

//Konzola
char Console::getc() {
    return ::getc();
}
void Console::putc (char ch)
{
    ::putc(ch);
}

