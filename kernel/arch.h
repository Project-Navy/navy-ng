#pragma once 

#if defined(__x86_64__)
#   include "hw/x86_64/com.h"
#   include "hw/x86_64/const.h"
#   include "hw/x86_64/ctx.h"
#   include "hw/x86_64/vmm.h"
#   include "hw/x86_64/pmm.h"
#   include "hw/x86_64/regs.h"
#else 
#   error "Unknown architecture"
#endif

#include <navy/handover.h>

void hardware_init(Handover *handover);
void raise_debug(void);
void hlt(void);
void cli(void);
void sti(void);
VmmSpace space_create(void);
VmmSpace get_address_space(void);
bool sched_is_init(void);