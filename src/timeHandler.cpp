#include "../lib/hw.h"
#include "../h/Thread.hpp"
#include "../lib/console.h"

extern "C" void timeHandler(){
    //Kernel::Thread::dispatch();
    __asm__ volatile ("csrci sip, 0x02");
    //console_handler();
}