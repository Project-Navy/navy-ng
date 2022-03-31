#include "rtc.h"

#include "hw/x86_64/asm.h"

static int is_updating(void)
{
    asm_out8(0x70, 0x0a);
    return asm_in8(0x71) & 0x80;
}

static uint8_t read(uint8_t reg)
{
    while (is_updating());

    asm_out8(0x70, reg);
    return asm_in8(0x71);
}

uint8_t rtc_sec(void)
{
    uint8_t sec = read(0);
    return (sec & 0xf) + ((sec / 0x10) * 0xa);
}