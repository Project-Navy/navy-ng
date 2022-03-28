#include "hw/x86_64/const.h"
#include "hw/x86_64/vmm.h"
#include <navy/lock.h>
#include <navy/result.h>

#ifdef KERNEL 
#   include "arch.h"
#endif

static DECLARE_LOCK(liballoc);

int liballoc_lock(void)
{
    LOCK(liballoc);
    return 0;
}

int liballoc_unlock(void)
{
    UNLOCK(liballoc);
    return 0;
}

void *liballoc_alloc(int pages)
{
#ifdef KERNEL 
    VmmSpace space = get_address_space();
    Range addr_range = unwrap_or_panic(pmm_alloc(pages * PAGE_SIZE));

    vmm_map_range(space, (Range) {
        .base = mmap_phys_to_io(addr_range.base),
        .length = pages * PAGE_SIZE
    }, addr_range, true);

    return (void *) mmap_phys_to_io(addr_range.base);
#else 
#   error "Unimplemented"
#endif
}

int liballoc_free(void* ptr, int pages)
{
#ifdef KERNEL 
    VmmSpace space = get_address_space();
    vmm_unmap_page(space, (uintptr_t) ptr);
    pmm_free((Range) {
        .base = mmap_io_to_phys((uintptr_t) ptr),
        .length = pages * PAGE_SIZE
    });
#else 
#   error "Unimplemented"
#endif
    return 0;
}