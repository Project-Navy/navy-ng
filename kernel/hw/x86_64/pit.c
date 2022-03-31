#include "pit.h"

#include "hw/x86_64/asm.h"

static uint32_t pit_read_count(void)
{
    asm_out8(0x43, 0);
    uint32_t counter = asm_in8(0x40);
    counter |= asm_in8(0x40) << 8;

    return counter;
}

void pit_init(int hz)
{
    int divisor = 1193180 / hz;

    asm_out8(0x43, 0x36);
    asm_out8(0x40, divisor & 0xff);
    asm_out8(0x40, (divisor >> 8) & 0xFF);
}

void pit_sleep(uint16_t ms)
{
    asm_out8(0x43, 0x30);
    asm_out8(0x40, ms & 0xff);
    asm_out8(0x40, (ms >> 8) & 0xff);

    while (pit_read_count() == 0);
}