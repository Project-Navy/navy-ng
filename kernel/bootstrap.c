#include "arch.h"

#include <navy/debug.h>
#include <navy/handover.h>

int _start(Handover *handover)
{
    hardware_init(handover);
    raise_debug();
    return 0;
}