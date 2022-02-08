#pragma once

#include <brutal/str.h>
#include <navy/range.h>

#define LIMIT_MEMORY_MAP_SIZE 64
#define LIMIT_MODULE_SIZE 64

enum mmap_type 
{
    MEMMAP_USABLE,                 
    MEMMAP_RESERVED,               
    MEMMAP_ACPI_RECLAIMABLE,       
    MEMMAP_ACPI_NVS,               
    MEMMAP_BAD_MEMORY,             
    MEMMAP_BOOTLOADER_RECLAIMABLE, 
    MEMMAP_KERNEL_AND_MODULES,     
    MEMMAP_FRAMEBUFFER,            
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

    Module modules[LIMIT_MODULE_SIZE];
    Memmap memmaps[LIMIT_MEMORY_MAP_SIZE];
} Handover;