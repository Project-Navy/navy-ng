#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct range
{
    uintptr_t base;
    size_t length;
    uintptr_t end;
} Range;