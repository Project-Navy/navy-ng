#include "mapping.h"

#include <assert.h>

#include <limine/limine.h>
#include <navy/debug.h>

static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

uintptr_t mmap_phys_to_kernel(uintptr_t phys_addr)
{
    assert(hhdm_request.response != NULL);
    return phys_addr + hhdm_request.response->offset;
}

uintptr_t mmap_kernel_to_phys(uintptr_t virt_addr)
{
    assert(hhdm_request.response != NULL);
    return virt_addr - hhdm_request.response->offset;
}