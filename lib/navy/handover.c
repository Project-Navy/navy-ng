#include <navy/handover.h>

void dump_mmap(Handover *handover)
{
    for (size_t i = 0; i < handover->memmap_count; i++)
    {
        log$("{a} - {a} ({})", handover->memmaps[i].range.base, handover->memmaps[i].range.length, 
            mmap_type_str[handover->memmaps[i].type]);
    }
}