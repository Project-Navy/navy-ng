#include "com.h"
#include "hw/x86_64/asm.h"

#include <stdbool.h>
#include <stdint.h>

static bool is_serial_init = false;

void serial_init(uint16_t port)
{
    asm_out8(port + 1, 0x00);
    asm_out8(port + 0, 0x03);
    asm_out8(port + 1, 0x00);
    asm_out8(port + 3, 0x03);
    asm_out8(port + 2, 0xc7);
    asm_out8(port + 4, 0x0b);
    asm_out8(port + 4, 0x1e);
    asm_out8(port + 0, 0xae);
    asm_out8(port + 4, 0x0f);

    is_serial_init = true;
}

void serial_putc(char c)
{
    if (!is_serial_init)
    {
        serial_init(SERIAL_COM1);
    }

    while ((asm_in8(SERIAL_COM1 + 5) & 0x20) == 0);

    asm_out8(SERIAL_COM1, c);
}

void serial_puts(char const *s)
{
    while (*s != '\0')
    {
        serial_putc(*s++);
    }
}