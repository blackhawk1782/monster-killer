#include "utilities/memory_pool.h"
#include <stdlib.h>
#include <string.h>

bool MemoryPool_Init(MemoryPool* pool, u32 elementSize, u32 capacity) {
    if (!pool || elementSize == 0 || capacity == 0) return false;
    pool->buffer = (u8*)calloc(capacity, elementSize);
    pool->active = (bool*)calloc(capacity, sizeof(bool));
    if (!pool->buffer || !pool->active) {
        free(pool->buffer);
        free(pool->active);
        return false;
    }
    pool->elementSize = elementSize;
    pool->capacity = capacity;
    pool->used = 0;
    return true;
}

void MemoryPool_Shutdown(MemoryPool* pool) {
    if (!pool) return;
    free(pool->buffer);
    free(pool->active);
    memset(pool, 0, sizeof(MemoryPool));
}

void* MemoryPool_Alloc(MemoryPool* pool) {
    if (!pool || pool->used >= pool->capacity) return NULL;
    for (u32 i = 0; i < pool->capacity; i++) {
        if (!pool->active[i]) {
            pool->active[i] = true;
            pool->used++;
            return pool->buffer + i * pool->elementSize;
        }
    }
    return NULL;
}

void MemoryPool_Free(MemoryPool* pool, void* ptr) {
    if (!pool || !ptr) return;
    ptrdiff_t offset = (u8*)ptr - pool->buffer;
    if (offset < 0 || offset >= (ptrdiff_t)(pool->capacity * pool->elementSize)) return;
    u32 idx = (u32)(offset / pool->elementSize);
    pool->active[idx] = false;
    pool->used--;
}

void MemoryPool_Clear(MemoryPool* pool) {
    if (!pool) return;
    memset(pool->active, 0, pool->capacity * sizeof(bool));
    pool->used = 0;
}
