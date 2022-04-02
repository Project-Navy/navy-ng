#include "pmm.h"
#include "const.h"

#include <navy/bitmap.h>
#include <navy/debug.h>
#include <navy/handover.h>
#include <navy/macro.h>
#include <navy/lock.h>

static Bitmap bitmap;
static DECLARE_LOCK(pmm);
static size_t last_index = 0;

void pmm_init(Handover *handover)
{
    Range last_memmap_range = handover->memmaps[handover->memmap_count - 1].range;
    bitmap.length = (last_memmap_range.base + last_memmap_range.length) / (PAGE_SIZE * 8);

    log$("PMM Bitmap requires {} bytes", bitmap.length);
    for (size_t i = 0; i < handover->memmap_count; i++)
    {
        Memmap *entry = &handover->memmaps[i];

        if (entry->type == MEMMAP_USABLE && entry->range.length >= bitmap.length)
        {
            log$("PMM Bitmap will use memmap at 0x{a}", entry->range.base);
            bitmap.buffer = (uint8_t *) mmap_phys_to_kernel(entry->range.base);

            entry->range.base += ALIGN_UP(bitmap.length, PAGE_SIZE);
            entry->range.length -= ALIGN_UP(bitmap.length, PAGE_SIZE);
            
            break;
        }
    }

    if (bitmap.buffer == NULL)
    {
        panic$("Couldn't find a valid memory space for the PMM Bitmap");
    }

    memset(bitmap.buffer, 0xff, bitmap.length);

    for (size_t i = 0; i < handover->memmap_count; i++)
    {
        Memmap entry = handover->memmaps[i];

        if (entry.type == MEMMAP_USABLE)
        {
            pmm_free((Range) {
                .base = ALIGN_DOWN(entry.range.base, PAGE_SIZE),
                .length = ALIGN_UP(entry.range.length, PAGE_SIZE)
            });
        }
    }

    pmm_set_used((Range) {
        .base = mmap_kernel_to_phys((PhysicalAddress) bitmap.buffer),
        .length = bitmap.length
    });
}

void pmm_set_used(Range page)
{
    LOCK(pmm);
    size_t target = page.base / PAGE_SIZE;

    for (size_t i = 0; i < page.length / PAGE_SIZE; i++)
    {
        bitmap_set_bit(&bitmap, target + i);
    }
    UNLOCK(pmm);
}

void pmm_free(Range page)
{
    LOCK(pmm);
    size_t target = page.base / PAGE_SIZE;

    for (size_t i = 0; i < page.length / PAGE_SIZE; i++)
    {
        bitmap_clear_bit(&bitmap, target + i);
    }
    UNLOCK(pmm);
}

PmmOption pmm_alloc(size_t count)
{
    Range range = {0};

    for (size_t i = last_index; i < bitmap.length && range.length < count; i++)
    {
        if (!bitmap_is_bit_set(&bitmap, i))
        {
            if (range.length == 0)
            {
                range.base = i * PAGE_SIZE;
            }

            range.length += PAGE_SIZE;
        }
        else 
        {
            range.length = 0;
        }
    }

    if (range.length >= count)
    {
        last_index = (range.base + range.length) / PAGE_SIZE; 
        pmm_set_used(range);
    }
    else 
    {
        if (last_index == 0)
        {
            return NONE(PmmOption);
        }

        last_index = 0;
        return pmm_alloc(count);
    }

    return SOME(PmmOption, range);
}

Range get_pmm_bitmap_range(void)
{
    return (Range) {
        .base = (uintptr_t) bitmap.buffer,
        .length = bitmap.length
    };
}