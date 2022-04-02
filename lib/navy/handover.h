#pragma once

#include <brutal/str.h>
#include <brutal/result.h>
#include <navy/range.h>
#include <navy/macro.h>
#include <navy/option.h>

#define LIMIT_MEMORY_MAP_SIZE 64
#define LIMIT_MODULE_SIZE 64

#define FOREACH_MEMMAP_TYPE(FUNC)         \
    FUNC(MEMMAP_USABLE)                   \
    FUNC(MEMMAP_RESERVED)                 \
    FUNC(MEMMAP_ACPI_RECLAIMABLE)         \
    FUNC(MEMMAP_ACPI_NVS)                 \
    FUNC(MEMMAP_BAD_MEMORY)               \
    FUNC(MEMMAP_BOOTLOADER_RECLAIMABLE)   \
    FUNC(MEMMAP_KERNEL_AND_MODULES)       \
    FUNC(MEMMAP_FRAMEBUFFER)

enum mmap_type 
{
    FOREACH_MEMMAP_TYPE(GENERATE_ENUM)
};

[[gnu::used]] static char const *mmap_type_str[] = {
    FOREACH_MEMMAP_TYPE(GENERATE_STRING)
};

typedef struct 
{
    Range range;
    enum mmap_type type;
} Memmap;

typedef struct 
{
    Str name;
    Range addr;
} Module;

typedef struct 
{
    size_t memmap_count;
    size_t module_count;

    Module modules[LIMIT_MODULE_SIZE];
    Memmap memmaps[LIMIT_MEMORY_MAP_SIZE];
} Handover;

typedef Option(Module *) ModuleOption;

void handover_dump_mmap(Handover *handover);
ModuleOption handover_find_module(Handover *handover, Str name);