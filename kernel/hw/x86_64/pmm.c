#include "pmm.h"
#include "const.h"

#include <navy/bitmap.h>
#include <navy/debug.h>
#include <navy/handover.h>
#include <navy/macro.h>

static PmmAlloc alloc;
static size_t last_index = 0;

void pmm_init(Handover *handover)
{
    Range last_memmap_range = handover->memmaps[handover->memmap_count - 1].range;

    alloc.bitmap.length = (last_memmap_range.base + last_memmap_range.length) / (PAGE_SIZE * 8);

    log$("PMM Bitmap requires {} bytes", alloc.bitmap.length);
    for (size_t i = 0; i < handover->memmap_count && alloc.bitmap.buffer == NULL; i++)
    {
        Memmap *entry = &handover->memmaps[i];

        if ((entry->type == MEMMAP_USABLE || entry->type == MEMMAP_BOOTLOADER_RECLAIMABLE) \
                && entry->range.length > 0)
        {
            log$("PMM Bitmap will use memmap at 0x{a}", entry->range.base);
            alloc.bitmap.buffer = (uint8_t *) entry->range.base;
        }

        entry->range.base += ALIGN_UP(alloc.bitmap.length, PAGE_SIZE);
        entry->range.length -= ALIGN_UP(alloc.bitmap.length, PAGE_SIZE);
    }

    if (alloc.bitmap.buffer == NULL)
    {
        panic$("Couldn't find a valid memory space for the PMM Bitmap");
    }

    memset(alloc.bitmap.buffer, 0xff, alloc.bitmap.length);

    for (size_t i = 0; i < handover->memmap_count; i++)
    {
        Memmap entry = handover->memmaps[i];

        if (entry.type == MEMMAP_USABLE || entry.type == MEMMAP_BOOTLOADER_RECLAIMABLE)
        {
            pmm_free((Range) {
                .base = ALIGN_DOWN(entry.range.base, PAGE_SIZE),
                .length = ALIGN_UP(entry.range.length, PAGE_SIZE)
            });
        }
    }

    pmm_set_used((Range) {
        .base = mmap_io_to_phys((PhysicalAddress) alloc.bitmap.buffer),
        .length = alloc.bitmap.length
    });
}

void pmm_set_used(Range page)
{
    // insert lock
    size_t target = page.base / PAGE_SIZE;

    for (size_t i = 0; i < page.length / PAGE_SIZE; i++)
    {
        bitmap_set_bit(&alloc.bitmap, target + i);
    }
}

void pmm_free(Range page)
{
    // insert lock
    size_t target = page.base / PAGE_SIZE;

    for (size_t i = 0; i < page.length / PAGE_SIZE; i++)
    {
        bitmap_clear_bit(&alloc.bitmap, target + i);
    }
}

PmmOption pmm_alloc(size_t count)
{
    // insert lock
    Range range = {0};

    for (size_t i = last_index; i < alloc.bitmap.length && range.length < count; i++)
    {
        if (bitmap_is_bit_set(&alloc.bitmap, i))
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
            return None(PmmOption);
        }
    }

    return Some(PmmOption, range);
}