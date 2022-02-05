#include <stdint.h>
#include <stivale/stivale2.h>

#include "arch.h"

static uint8_t stack[STACK_SIZE];
void stivale2_entry(struct stivale2_struct *stivale2);
extern void _start(void);

[[gnu::section(".stivale2hdr"), gnu::used]] static struct stivale2_header stivale_hdr = {
    .entry_point = (uintptr_t) stivale2_entry,
    .stack = (uintptr_t) stack + sizeof(stack),
    .flags = (1 << 1),
    .tags = 0,
};

void stivale2_entry([[maybe_unused]] struct stivale2_struct *stivale2)
{
    serial_puts("\033[2J\033[H");
    _start();
}