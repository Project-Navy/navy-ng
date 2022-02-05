#pragma once

#include <brutal/str.h>
#include <navy/range.h>

#define LIMIT_MEMORY_MAP_SIZE 64
#define LIMIT_MODULE_SIZE 64

enum mmap_type 
{
    USABLE,                 
    RESERVED,               
    ACPI_RECLAIMABLE,       
    ACPI_NVS,               
    BAD_MEMORY,             
    BOOTLOADER_RECLAIMABLE, 
    KERNEL_AND_MODULES,     
    FRAMEBUFFER,            
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
    Module modules[LIMIT_MODULE_SIZE];
    Memmap memmaps[LIMIT_MEMORY_MAP_SIZE];
} Handover;