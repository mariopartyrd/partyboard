#include "msm/msmmem.h"

typedef struct MSMBlock_s {
    struct MSMBlock_s* prev;
    struct MSMBlock_s* next;
    u32 freeSize;
    u32 size;
    void* ptr;
#ifdef TARGET_PC
    // this looks bad but hear me out... keep allocations 32B aligned with 64 bit pointers
    char pad[64 - 3 * sizeof(void*) - 2 * sizeof(u32)];
#else
    char pad[12];
#endif
} MSMBLOCK;

typedef struct MSMMem_s {
    void *ptr;
    u32 size;
    MSMBLOCK *head;
    MSMBLOCK first;
} MSM_MEM;

static MSM_MEM mem;

void msmMemFree(void* ptr) {
    MSMBLOCK* block;
    MSMBLOCK* blockPrev;
    MSMBLOCK* blockNext;
    MSMBLOCK* blockHead;
    MSMBLOCK* base;
    
    base = ptr;

    block = &base[-1];
    blockPrev = block->prev;
    blockNext = block->next;
    if (mem.ptr > block || ((uintptr_t)mem.ptr + (u32)mem.size) <= (uintptr_t)block) {
        return;
    }

    if ((blockPrev->next != block) || (block->ptr != ptr) || (blockNext && (blockNext->prev != block))) {
        return;
    }
    
    blockPrev->size += block->freeSize + block->size;
    blockPrev->next = blockNext;
    blockHead = mem.head;
    if ((blockHead == block) || (blockHead->size < blockPrev->size)) {
        mem.head = blockPrev;
    }
    if (blockNext) {
        blockNext->prev = blockPrev;
        if (mem.head->size < blockNext->size) {
            mem.head = blockNext;
        }
    }
}

void* msmMemAlloc(u32 size) {
    s32 alignOfs;
    u32 freeSize;
    u32 allocSize;
    MSMBLOCK* block;
    MSMBLOCK* blockPrev;
    MSMBLOCK* blockNext;

    allocSize = size + sizeof(MSMBLOCK);
    alignOfs = allocSize & 0x1F;
    if (alignOfs) {
        allocSize += 0x20 - alignOfs;
    }
    if (mem.head->size >= allocSize) {
        blockPrev = mem.head;
    } else {
        blockPrev = &mem.first;
        
       do {
            if (blockPrev->size >= allocSize) break;
            blockPrev = blockPrev->next;
        } while (blockPrev);
        if (!blockPrev) {
            return NULL;
        }
    }
    
    freeSize = blockPrev->freeSize;
    if (freeSize != 0) {
        freeSize -= sizeof(MSMBLOCK);
    }
    block = (void*)((uintptr_t)blockPrev->ptr + (freeSize));
    blockNext = blockPrev->next;
    if ((mem.ptr > block) || ((void*)((uintptr_t)mem.ptr + (u32)mem.size) <= block)) {
        return NULL;
    }
    block->freeSize = allocSize;
    block->size = blockPrev->size - allocSize;
    block->ptr = (block + 1);
    block->prev = blockPrev;
    block->next = blockNext;
    mem.head = block;
    blockPrev->size = 0;
    blockPrev->next = block;
    if (blockNext) {
        blockNext->prev = block;
        if (mem.head->size < blockNext->size) {
            mem.head = blockNext;
        }
    }
    return block->ptr;
}

void msmMemInit(void* ptr, u32 size) {
    MSMBLOCK* block;
    s32 ofs;

    ofs = (uintptr_t)ptr & 0x1F;
    switch (ofs) {
        default:
            ofs = 0x20 - ofs;
            break;
        case 0:
            ofs = 0;
            break;
    }
    
    mem.ptr = (void*)((uintptr_t)ptr + ofs);
#ifdef TARGET_PC
    mem.size = ((uintptr_t)ptr + size - (uintptr_t)mem.ptr) & ~0x1F;
#else
    ofs = (s32)ptr + size;
    mem.size = ((ofs - (s32)mem.ptr) & ~0x1F);
#endif
    block = &mem.first;
    block->freeSize = 0;
    block->size = mem.size;
    block->ptr = mem.ptr;
    block->prev = NULL;
    block->next = NULL;
    mem.head = &mem.first;
}
