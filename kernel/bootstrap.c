#include "arch.h"

#include <stdlib.h>

#include <navy/debug.h>
#include <navy/handover.h>

int _start(Handover *handover)
{
    hardware_init(handover);

    for(;;);
    return 0;
}