#include "arch.h"

#include <stdlib.h>

#include <navy/debug.h>
#include <navy/handover.h>
#include <limine/limine.h>

static Handover handover;

int _start(void)
{
    serial_puts("\033[2J\033[H");

    parse_mmap(&handover);
    dump_mmap(&handover);
    hardware_init(&handover);
    raise_debug();

    return 0;
}