#pragma once

#include "navy/lock.h"
#include <brutal/result.h>
#include <navy/range.h>
#include <navy/handover.h>
#include <navy/bitmap.h>

typedef Option(Range) PmmOption;

PmmOption pmm_alloc(size_t size);
void pmm_free(Range page);
void pmm_set_used(Range page);
void pmm_init(Handover *handover);
Range get_pmm_bitmap_range(void);