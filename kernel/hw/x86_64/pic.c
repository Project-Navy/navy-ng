#include "pic.h"
#include "hw/x86_64/asm.h"

void pic_init(void)
{
    asm_out8(MASTER_CMD, 0x10 | 0x01);
    io_wait();
    asm_out8(SLAVE_CMD, 0x10 | 0x01);
    io_wait();

    asm_out8(MASTER_DATA, MASTER_OFFSET);
    io_wait();
    asm_out8(SLAVE_DATA, SLAVE_OFFSET);
    io_wait();

    asm_out8(MASTER_DATA, 4);
    io_wait();
    asm_out8(SLAVE_DATA, 2);
    io_wait();

    asm_out8(MASTER_DATA, 0x01);
    io_wait();
    asm_out8(SLAVE_DATA, 0x01);
    io_wait();

    asm_out8(MASTER_DATA, 0);
    io_wait();
    asm_out8(SLAVE_DATA, 0);
    io_wait();
}

void pic_sendEOI(int intno)
{
    if (intno >= 40)
    {
        asm_out8(SLAVE_CMD, 0x20);
    }

    asm_out8(MASTER_CMD, 0x20);
}