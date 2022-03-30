#pragma once 

#include <stdint.h>

void init_mapping(void);
uintptr_t mmap_phys_to_kernel(uintptr_t phys_addr);
uintptr_t mmap_kernel_to_phys(uintptr_t virt_addr);