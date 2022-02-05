#pragma once 

#include <stdint.h>

#define STIVALE2_HEADER_TAG_ANY_VIDEO_ID 0xc75c9fa92a44c4db
#define STIVALE2_HEADER_TAG_FRAMEBUFFER_ID 0x3ecc1bc43d0f7971
#define STIVALE2_HEADER_TAG_FB_MTRR_ID 0x4c7bb07731282e00
#define STIVALE2_HEADER_TAG_SLIDE_HHDM_ID 0xdc29269c2af53d1d
#define STIVALE2_HEADER_TAG_TERMINAL_ID 0xa85d499b1823be72
#define STIVALE2_HEADER_TAG_SMP_ID 0x1ab015085f3273df
#define STIVALE2_HEADER_TAG_5LV_PAGING_ID 0x932f477032007e8f
#define STIVALE2_HEADER_TAG_UNMAP_NULL_ID 0x92919432b16fe7e7
#define STIVALE2_STRUCT_TAG_PMRS_ID 0x5df266a64047b6bd
#define STIVALE2_STRUCT_TAG_KERNEL_BASE_ADDRESS_ID 0x060d78874a2a8af0
#define STIVALE2_STRUCT_TAG_CMDLINE_ID 0xe5e76a1b4597a781
#define STIVALE2_STRUCT_TAG_MEMMAP_ID 0x2187f79e8612de07
#define STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID 0x506461d2950408fa
#define STIVALE2_STRUCT_TAG_EDID_ID 0x968609d7af96b845
#define STIVALE2_STRUCT_TAG_TEXTMODE_ID 0x38d74c23e0dca893
#define STIVALE2_STRUCT_TAG_FB_MTRR_ID 0x6bc1a78ebe871172
#define STIVALE2_STRUCT_TAG_TERMINAL_ID 0xc2b3f4c3233b0974
#define STIVALE2_STRUCT_TAG_MODULES_ID 0x4b6fe466aade04ce
#define STIVALE2_STRUCT_TAG_RSDP_ID 0x9e1786930a375e78
#define STIVALE2_STRUCT_TAG_EPOCH_ID 0x566a7bed888e1407
#define STIVALE2_STRUCT_TAG_FIRMWARE_ID 0x359d837855e3858c
#define STIVALE2_STRUCT_TAG_EFI_SYSTEM_TABLE_ID 0x4bc5ec15845b558e
#define STIVALE2_STRUCT_TAG_KERNEL_FILE_ID 0xe599d90c2975584a
#define STIVALE2_STRUCT_TAG_KERNEL_FILE_V2_ID 0x37c13018a02c6ea2
#define STIVALE2_STRUCT_TAG_BOOT_VOLUME_ID 0x9b4358364c19ee62
#define STIVALE2_STRUCT_TAG_KERNEL_SLIDE_ID 0xee80847d01506c57
#define STIVALE2_STRUCT_TAG_SMBIOS_ID 0x274bd246c62bf7d1
#define STIVALE2_STRUCT_TAG_SMP_ID 0x34d1d96339647025
#define STIVALE2_STRUCT_TAG_PXE_SERVER_INFO 0x29d1e96239247032
#define STIVALE2_STRUCT_TAG_MMIO32_UART 0xb813f9b8dbc78797
#define STIVALE2_STRUCT_TAG_DTB 0xabb29bd49a2833fa
#define STIVALE2_STRUCT_TAG_HHDM_ID 0xb0ed257db18cb58f

struct stivale2_header 
{
    uintptr_t entry_point;
    uintptr_t stack;
    uint64_t flags;
    uintptr_t tags;
};

struct stivale2_tag 
{
    uint64_t identifier;
    uint64_t next;
};

#define memmap_type \
    transform(STIVALE2_MMAP_USABLE,                 1) \
    transform(STIVALE2_MMAP_RESERVED,               2) \
    transform(STIVALE2_MMAP_ACPI_RECLAIMABLE,       3) \
    transform(STIVALE2_MMAP_ACPI_NVS,               4) \
    transform(STIVALE2_MMAP_BAD_MEMORY,             5) \
    transform(STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE, 0x1000) \
    transform(STIVALE2_MMAP_KERNEL_AND_MODULES,     0x1001) \
    transform(STIVALE2_MMAP_FRAMEBUFFER,            0x1002)

#define transform(x, y) x = y,
enum stivale2_memmap_type { memmap_type };
#undef transform

#define transform(x, y) [y] = #x,
static char const *stivale2_memmap_str[] = { memmap_type };
#undef transform

struct stivale2_mmap_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t unused;
};

struct stivale2_struct_tag_memmap 
{
    struct stivale2_tag tag;
    uint64_t entries;
    struct stivale2_mmap_entry memmap[];
};


#define STIVALE2_BOOTLOADER_VERSION_SIZE 64
#define STIVALE2_BOOTLOADER_BRAND_SIZE 64

struct stivale2_struct 
{
    char bootloader_brand[STIVALE2_BOOTLOADER_BRAND_SIZE];
    char bootloader_version[STIVALE2_BOOTLOADER_VERSION_SIZE];
    uint64_t tags;
};


#define STIVALE2_MODULE_STRING_SIZE 128

struct stivale2_module {
    uint64_t begin;
    uint64_t end;
    char string[STIVALE2_MODULE_STRING_SIZE];
};

struct stivale2_struct_tag_modules 
{
    struct stivale2_tag tag;
    uint64_t module_count;
    struct stivale2_module modules[];
};