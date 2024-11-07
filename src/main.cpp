extern  "C" void supervisorTrap();
#include "../test/userMain.hpp"
#include "../h/syscall_cpp.hpp"

void userMainWrapper(void* arg)
{
    userMain();
}

void main(){
    //inicijalizacije
    MemoryAllocator::init();
    Kernel::Scheduler::init();

    __asm__ volatile("csrw stvec, %0": : "r"((uint64)&supervisorTrap|1));//setovanje tabele prekida
    __asm__ volatile ("csrs sstatus, 0x2");//dozvoljeni softverski prekidi
    __asm__ volatile("csrc sstatus, %0": : "r"((uint64)256));//prelazak u korisnicki rezim

    //kreiranje main niti
    thread_t maint;
    thread_create(&maint,nullptr,nullptr);
    Kernel::Thread::running=maint;

    //kreiranje test niti
    thread_t testThread;
    thread_create(&testThread,userMainWrapper,nullptr);

    //cekanje test niti
    thread_join(testThread);
}
