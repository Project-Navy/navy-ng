#include <limine/limine.h>
#include <navy/handover.h>
#include <navy/debug.h>

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void parse_mmap(Handover *handover)
{
    if (memmap_request.response == NULL)
    {
        panic$("Can't get memmaps !");
    }

    Memmap *m;
    handover->memmap_count = memmap_request.response->entry_count;

    for (size_t i = 0; i < memmap_request.response->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];
        m = &handover->memmaps[i];

        switch(entry->type)
        {
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            {
                m->type = MEMMAP_BOOTLOADER_RECLAIMABLE;
                break;
            }

            case LIMINE_MEMMAP_USABLE:
            {
                m->type = MEMMAP_USABLE;
                break;
            }

            case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            {
                m->type = MEMMAP_KERNEL_AND_MODULES;
                break;
            }

            case LIMINE_MEMMAP_FRAMEBUFFER:
            {
                m->type = MEMMAP_FRAMEBUFFER;
                break;
            }

            default: 
            {
                m->type = MEMMAP_RESERVED;
            }
        }

        m->range = (Range) {.base = entry->base, .length = entry->length};
    }
}