#include "../h/MemoryAllocator.hpp"


MemoryAllocator::FreeMem* MemoryAllocator::head=(FreeMem*) HEAP_START_ADDR;

MemoryAllocator::MemoryAllocator() { }

int MemoryAllocator::tryToJoin(FreeMem* cur){
    if(!cur) return 0;
    if(cur->next && (char*)cur+cur->size == (char*)(cur->next)){
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if(cur->next) cur->next->prev = cur;
        return 1;
    }else{
        return 0;
    }
}

void MemoryAllocator::init() {
    head=(FreeMem*) HEAP_START_ADDR;
    head->next= nullptr;
    head->prev= nullptr;
    head->size=(char*) HEAP_END_ADDR - (char*) HEAP_START_ADDR;
}

void* MemoryAllocator::malloc(size_t sz){
    size_t  size = sz * MEM_BLOCK_SIZE;

    if (size == 0) return nullptr;

    FreeMem *blk = head;
    FreeMem *prev = nullptr;
    for (; blk!=nullptr; prev = blk, blk = blk->next) {
        if(blk->size >= size) break;
    }
    if(blk== nullptr){
        return nullptr;
    }
    size_t remainingSize = blk->size - size;
    if(remainingSize >= MEM_BLOCK_SIZE){
        blk->size = size;
        size_t offset = size;
        FreeMem* newBlk = (FreeMem*)((char*)blk+offset);
        if(prev) prev->next = newBlk;
        else head = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize;
    }else{
        if(prev) prev->next = blk->next;
        else head = blk->next;
    }
    blk->next = nullptr;
    char* n;
    n = (char*) blk;
    *n = sz;
    return n+1;
}

int MemoryAllocator::free(void *addr1) {
    char* addr=(char*) addr1-1;
    if(addr1== nullptr || addr == nullptr) return -1;
    if(addr1<(char*)HEAP_START_ADDR || addr1>(char*)HEAP_END_ADDR) return -1;
    FreeMem* cur=0;
    size_t size=(size_t)((int)(*addr))*MEM_BLOCK_SIZE;
    if(!head || addr<(char*)head) cur=0;
    else for (cur = head; cur->next!=0 &&addr>(char*)(cur->next) ; cur=cur->next);

    FreeMem * newSeg=(FreeMem*)addr;
    newSeg->size=size;
    newSeg->prev=cur;
    if(cur) newSeg->next=cur->next;
    else newSeg->next=head;
    if(newSeg->next) newSeg->next->prev=newSeg;
    if(cur) cur->next=newSeg;
    else head=newSeg;
    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;
}