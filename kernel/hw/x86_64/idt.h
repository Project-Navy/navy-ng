#pragma once

#include <stdint.h>

struct [[gnu::packed]] idt_descr
{
    uint16_t offset_1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
};

enum idt_attr
{
    INTGATE = 0x8e,
    TRAPGATE = 0xef
};

struct [[gnu::packed]] idt_ptr
{
    uint16_t limit;
    uintptr_t base;
};

void idt_init(void);
void idt_reload(void);
extern void idt_flush(uintptr_t kidt_ptr);