#include "arch.h"

#include "gdt.h"
#include "idt.h"
#include "simd.h"

void hardware_init([[maybe_unused]] Handover *handover)
{
    gdt_init();
    idt_init();
    simd_init();
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