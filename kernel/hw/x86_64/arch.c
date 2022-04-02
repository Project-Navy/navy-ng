#include "arch.h"

#include "gdt.h"
#include "idt.h"
#include "simd.h"
#include "pmm.h"
#include "vmm.h"
#include "pit.h"
#include "pic.h"

#include "hw/x86_64/asm.h"

extern void syscall_handle(void);

static void syscall_init(void)
{
    asm_write_msr(MSR_EFER, asm_read_msr(MSR_EFER) | 1);
    asm_write_msr(MSR_STAR, ((uint64_t)(GDT_KERNEL_CODE * 8) << STAR_KCODE_OFFSET) | ((uint64_t) (((GDT_USER_DATA - 1) * 8) | 3 ) << STAR_UCODE_OFFSET));
    asm_write_msr(MSR_LSTAR, (uint64_t) syscall_handle);
    asm_write_msr(MSR_SYSCALL_FLAG_MASK, 0);
}

void hardware_init(Handover *handover)
{
    gdt_init();
    idt_init();
    simd_init();
    pmm_init(handover);
    vmm_init(handover);
    intstack_init();
    pit_init(1000);
    pic_init();
    syscall_init();
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
