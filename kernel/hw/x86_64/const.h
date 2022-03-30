#pragma once 

#include <stdint.h>

#define STACK_SIZE (0x4000)
#define PAGE_SIZE (4096)

#define MMAP_IO_BASE ((uintptr_t) 0xffff800000000000)
#define MMAP_KERNEL_BASE ((uintptr_t) 0xffffffff80000000)

static inline uintptr_t mmap_phys_to_io(uintptr_t phys_addr)
{
    return phys_addr + MMAP_IO_BASE;
}

static inline uintptr_t mmap_io_to_phys(uintptr_t vaddr)
{
    return vaddr - MMAP_IO_BASE;
}

static inline uintptr_t mmap_phys_to_kernel(uintptr_t phys_addr)
{
    return phys_addr + MMAP_KERNEL_BASE;
}

static inline uintptr_t mmap_kernel_to_phys(uintptr_t vaddr)
{
    return vaddr - MMAP_KERNEL_BASE;
}