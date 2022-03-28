#include "arch.h"

#include <navy/debug.h>
#include <navy/handover.h>

int _start(Handover *handover)
{
    hardware_init(handover);
    log$("Ok");
    for(;;);
    return 0;
}