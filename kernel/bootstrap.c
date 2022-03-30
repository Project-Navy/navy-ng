#include "arch.h"

#include <stdlib.h>

#include <navy/debug.h>
#include <navy/handover.h>

int _start(Handover *handover)
{
    serial_puts("\033[2J\033[H");

    dump_mmap(handover);
    hardware_init(handover);
    raise_debug();

    return 0;
}