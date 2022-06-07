#pragma once 

#include <navy/range.h>

typedef struct 
{
    Range range;
    uintptr_t ref_count;
} SharedMemory;

SharedMemory *shared_memory_create(size_t size);