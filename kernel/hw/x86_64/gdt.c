#include "gdt.h"

static struct gdt gdt;
static struct gdt_pointer ptr = {.limit = sizeof(struct gdt) - 1, .base = (uintptr_t) &gdt};
static struct tss tss = {0};

union gdt_entry gdt_entry_get(size_t index)
{
    return gdt.entries[index];
}

void init_descriptor(union gdt_entry *this, uint32_t base, uint32_t limit, uint8_t access)
{
    if (access != GDT_DATA)
    {
        this->flags = 0b0010;
    }
    else
    {
        this->flags = 0;
    }

    this->access = 0b10000010 | access;

    this->base_low = base & 0xffff;
    this->base_mid = (base & 0xff0000) >> 16;
    this->base_high = (base & 0xff000000) >> 24;

    this->limit_low = limit & 0xffff;
    this->limit_high = (limit & 0xf0000) >> 16;
}

static struct tss_entry init_tss(uintptr_t tss)
{
    return (struct tss_entry)
    {
        .length = sizeof(struct tss_entry),
        .base_low16 = (uint16_t) tss & 0xffff,
        .base_mid8 = (uint8_t) (tss >> 16 ) & 0xff,
        .flags1 = 0b10001001,
        .flags2 = 0,
        .base_high8 = (uint8_t) (tss >> 24) & 0xff,
        .base_upper32 = tss >> 32,
        .reserved = 0,
    };
}

void gdt_init(void)
{
    init_descriptor(&gdt.entries[0], 0, 0, 0);

    init_descriptor(&gdt.entries[1], 0, 0, GDT_KERNEL | GDT_CODE);
    init_descriptor(&gdt.entries[2], 0, 0, GDT_KERNEL | GDT_DATA);

    init_descriptor(&gdt.entries[3], 0, 0, GDT_USER | GDT_DATA);
    init_descriptor(&gdt.entries[4], 0, 0, GDT_USER | GDT_CODE);

    gdt.tss = init_tss((uintptr_t) &tss);
    tss.iopb = sizeof(struct tss);

    gdt_reload();
    tss_flush();
}

void gdt_reload(void)
{
    __asm__ volatile("cli");
    gdt_flush((uintptr_t) &ptr);
    __asm__ volatile("sti");
}