#include "arch.h"

#include "gdt.h"
#include "idt.h"

void hardware_init(void)
{
    gdt_init();
    idt_init();
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