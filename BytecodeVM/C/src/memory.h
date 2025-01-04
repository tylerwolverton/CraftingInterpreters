#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define INIT_ARRAY(type, pointer, newCount) \
    (type*)reallocate(pointer, 0, sizeof(type) * (newCount))

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif