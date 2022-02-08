#include "vmm.h"
#include "pmm.h"
#include "const.h"

#include <assert.h>
#include <navy/range.h>
#include <hw/x86_64/asm.h>

static Pml *kernel_pml;
static size_t limit_pml = 4; // TODO: Detect 5 level

static void vmm_flush(Range virt_range)
{
    for (size_t i = 0; i < (virt_range.length / PAGE_SIZE); i++)
    {
        asm_invlpg(((virt_range.base / PAGE_SIZE) + 1) * PAGE_SIZE);
    }
}

static Range vmm_get_pml_alloc(Pml *pml, size_t index, bool is_user)
{
    PmlEntry entry = pml->entries[index];

    if (entry.present)
    {
        Range pml_range = {mmap_phys_to_io(entry.physical << 12), PAGE_SIZE};
        return pml_range;
    }
    else  
    {
        Range target_range = unwrap_or_panic(pmm_alloc(PAGE_SIZE));
        memset((void *) mmap_phys_to_io(target_range.base), 0, PAGE_SIZE);
        pml->entries[index] = pml_make_entry(target_range.base, is_user);

        return (Range) {mmap_phys_to_io(target_range.base), PAGE_SIZE};
    }
}

static void vmm_map_page(Pml *pml, VirtualAddress virt, PhysicalAddress phys, bool is_user)
{
    Pml *last_entry = pml;

    for (size_t i = limit_pml - 1; i > 0; i--)
    {
        Range pml_range = vmm_get_pml_alloc(last_entry, PMLX_GET_INDEX(virt, i), true);
        last_entry = (Pml *) pml_range.base;
    }

    last_entry->entries[PMLX_GET_INDEX(virt, 0)] = pml_make_entry(phys, is_user);
}

void vmm_map_range(Pml *pml, Range virt, Range phys, bool is_user)
{
    // Infinite loop here
    assert(virt.length == phys.length);

    size_t physaddr;
    size_t virtaddr;

    for (size_t i = 0; i < (virt.length / PAGE_SIZE); i++)
    {
        physaddr = i * PAGE_SIZE + ALIGN_DOWN(phys.base, PAGE_SIZE);
        virtaddr = i * PAGE_SIZE + ALIGN_DOWN(virt.base, PAGE_SIZE);

        vmm_map_page(pml, virtaddr, physaddr, is_user);
    }

    vmm_flush(virt);
}

void vmm_switch_space(Pml *pml)
{
    write_cr3(mmap_io_to_phys((PhysicalAddress) pml));
}

void vmm_init(Handover *handover)
{
    Range kernel_range = unwrap_or_panic(pmm_alloc(PAGE_SIZE));
    kernel_pml = (Pml *) mmap_phys_to_io(kernel_range.base);
    memset(kernel_pml, 0, kernel_range.length);

    vmm_map_range(kernel_pml, 
        (Range) {
            .base = mmap_phys_to_io(0),
            .length = 0xffffffff,
        },
        (Range) {
            .base = 0,
            .length = 0xffffffff,
        }, false);

    for (size_t i = 0; i < handover->memmap_count; i++)
    {
        Memmap memmap = handover->memmaps[i];

        if (memmap.type == MEMMAP_KERNEL_AND_MODULES)
        {
            vmm_map_range(kernel_pml,
                (Range){
                    .base = mmap_phys_to_kernel(ALIGN_DOWN(memmap.range.base, PAGE_SIZE)),
                    .length = ALIGN_UP(memmap.range.length, PAGE_SIZE) + PAGE_SIZE,
                },
                (Range) {
                    .base = ALIGN_DOWN(memmap.range.base, PAGE_SIZE),
                    .length = ALIGN_UP(memmap.range.length, PAGE_SIZE) + PAGE_SIZE,
                },  false);
        }

        vmm_map_range(kernel_pml, 
        (Range){
            .base = mmap_phys_to_io(ALIGN_DOWN(memmap.range.base, PAGE_SIZE)),
            .length = ALIGN_UP(memmap.range.length, PAGE_SIZE) + PAGE_SIZE,
        },
                            
        (Range) {
            .base = ALIGN_DOWN(memmap.range.base, PAGE_SIZE),
            .length = ALIGN_UP(memmap.range.length, PAGE_SIZE) + PAGE_SIZE,
        }, false);
    }

    vmm_switch_space(kernel_pml);
}