#include <assert.h>
#include <stdint.h>
#include <stivale/stivale2.h>
#include <navy/handover.h>
#include <navy/debug.h>
#include <navy/range.h>

#include "arch.h"

static uint8_t stack[STACK_SIZE];
void stivale2_entry(struct stivale2_struct *stivale2);
extern void bootstrap(Handover *handover);

static struct stivale2_header_tag_framebuffer fb_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },

    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};


[[gnu::section(".stivale2hdr"), gnu::used]] static struct stivale2_header stivale_hdr = {
    .entry_point = (uintptr_t) stivale2_entry,
    .stack = (uintptr_t) stack + sizeof(stack),
    .flags = (1 << 1),
    .tags = (uintptr_t) &fb_tag,
};

static void stivale2_parse_mmap(Handover *handover, struct stivale2_struct_tag_memmap *memmap)
{
    struct stivale2_mmap_entry mmap_entry;
    assert(memmap->entries <= LIMIT_MEMORY_MAP_SIZE);
    handover->memmap_count = memmap->entries;
    Memmap *m;

    log$("Memory map:")
    for (size_t i = 0; i < memmap->entries ; i++)
    {
        m = &handover->memmaps[i];
        mmap_entry = memmap->memmap[i];

        switch(mmap_entry.type)
        {
            case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE: 
            {
                m->type = MEMMAP_BOOTLOADER_RECLAIMABLE;
                break;
            }

            case STIVALE2_MMAP_KERNEL_AND_MODULES:
            {
                m->type = MEMMAP_KERNEL_AND_MODULES;
                break;
            }

            case STIVALE2_MMAP_FRAMEBUFFER:
            {
                m->type = MEMMAP_FRAMEBUFFER;
                break;
            }


            case STIVALE2_MMAP_USABLE:
            {
                m->type = MEMMAP_USABLE;
                break;
            }

            default:
            {
                m->type = MEMMAP_RESERVED;
            }
        }

        m->range = (Range) {mmap_entry.base, mmap_entry.length};
    }
}

static void stivale2_parse_module(Handover *handover, struct stivale2_struct_tag_modules *modules)
{
    assert(modules->module_count <= LIMIT_MODULE_SIZE);
    Module *m;

    handover->module_count = modules->module_count;

    for (size_t i = 0; i < modules->module_count; i++)
    {
        m = &handover->modules[i];
        struct stivale2_module module = modules->modules[i];

        m->addr = (Range) {module.begin, module.end - module.begin};
        m->name = str$(module.string);

        log$("Found module named {}", m->name);
    }
}

static Handover stivale2_parse_header(struct stivale2_struct *stivale2)
{
    struct stivale2_tag *tag = (struct stivale2_tag *) ((uintptr_t) stivale2->tags);
    Handover handover;

    while (tag != NULL)
    {
        switch (tag->identifier)
        {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
            {
                stivale2_parse_mmap(&handover, (struct stivale2_struct_tag_memmap *) tag);
                break;
            }

            case STIVALE2_STRUCT_TAG_MODULES_ID:
            {
                stivale2_parse_module(&handover, (struct stivale2_struct_tag_modules *) tag);
                break;
            }
        }

        tag = (struct stivale2_tag *) ((uintptr_t) tag->next);
    }

    return handover;
}

void stivale2_entry(struct stivale2_struct *stivale2)
{
    // serial_puts("\033[2J\033[H");

    Handover handover = stivale2_parse_header(stivale2);
    bootstrap(&handover);
}