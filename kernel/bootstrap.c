#include "arch.h"
#include <navy/debug.h>

int _start(void)
{
    hardware_init();
    raise_debug();
    return 0;
}