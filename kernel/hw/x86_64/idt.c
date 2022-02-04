#include "idt.h"

#include <stddef.h>
#include <string.h>

struct idt_descr kidt[256] = {};
struct idt_ptr kidtr;
extern uintptr_t __interrupt_vector[];

void idt_descr_init(uintptr_t offset, enum idt_attr type, struct idt_descr *this, uint8_t ist)
{
    this->offset_1 = offset;
    this->offset_2 = (offset >> 16);
    this->offset_3 = (offset >> 32);

    this->ist = ist;
    this->selector = 0x08;
    this->type_attr = type;
    this->zero = 0;
}

void idt_init(void)
{
    memset(kidt, 0, 256 * sizeof(struct idt_descr));

    for (size_t i = 0; i < 3; i++)
    {
        idt_descr_init(__interrupt_vector[i], INTGATE, &kidt[i], 0);
    }

    idt_descr_init(__interrupt_vector[3], TRAPGATE, &kidt[3], 0);
    idt_descr_init(__interrupt_vector[4], TRAPGATE, &kidt[4], 0);

    for (size_t i = 5; i < 48; i++)
    {
        idt_descr_init(__interrupt_vector[i], INTGATE, &kidt[i], 0);
    }

    kidtr.base = (uintptr_t) &kidt[0];
    kidtr.limit = sizeof(kidt) - 1;

    idt_reload();
}

void idt_reload(void)
{
    idt_flush((uintptr_t) &kidtr);
}