//
// Created by os on 5/22/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#include "../lib/hw.h"

class MemoryAllocator {
public:
    static void* malloc(size_t sz);
    static int free(void* addr1);
    static void init();
private:
    typedef struct FreeMem{
        FreeMem* prev;
        FreeMem* next;
        size_t size;
    }FreeMem;
    static FreeMem* head;
    MemoryAllocator();
    static int tryToJoin(FreeMem*cur);
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
