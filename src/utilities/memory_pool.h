#ifndef MK_MEMORY_POOL_H
#define MK_MEMORY_POOL_H

#include "types.h"

typedef struct {
    u8* buffer;
    u32 elementSize;
    u32 capacity;
    u32 used;
    bool* active;
} MemoryPool;

bool MemoryPool_Init(MemoryPool* pool, u32 elementSize, u32 capacity);
void MemoryPool_Shutdown(MemoryPool* pool);
void* MemoryPool_Alloc(MemoryPool* pool);
void  MemoryPool_Free(MemoryPool* pool, void* ptr);
void  MemoryPool_Clear(MemoryPool* pool);

#endif
