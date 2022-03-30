#include "arch.h"

#include "gdt.h"
#include "idt.h"
#include "simd.h"
#include "pmm.h"
#include "vmm.h"
#include "pit.h"
#include "pic.h"

#include <hw/x86_64/asm.h>

void hardware_init(Handover *handover)
{
    gdt_init();
    idt_init();
    simd_init();
    pmm_init(handover);
    vmm_init(handover);
    pit_init(1000);
    pic_init();
}

void raise_debug(void)
{
    asm volatile("int $1");
}

void hlt(void)
{
    asm volatile("hlt");
}

void cli(void)
{
    asm volatile("cli");
}

void sti(void)
{
    asm volatile("sti");
}

VmmSpace get_address_space(void)
{
    return (VmmSpace) mmap_phys_to_kernel(read_cr3());
}