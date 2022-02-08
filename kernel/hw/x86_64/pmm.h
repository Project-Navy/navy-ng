#pragma once

#include <navy/result.h>
#include <navy/range.h>
#include <navy/handover.h>
#include <navy/bitmap.h>

typedef Result(Range, int) PmmResult;

typedef struct 
{
    // lock;
    Bitmap bitmap;
} PmmAlloc;

typedef uintptr_t PhysicalAddress;

PmmResult pmm_alloc(size_t count);
void pmm_free(Range page);
void pmm_set_used(Range page);
void pmm_init(Handover *handover);