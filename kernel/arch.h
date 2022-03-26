#pragma once 

#if defined(__x86_64__)
#   include "hw/x86_64/com.h"
#   include "hw/x86_64/const.h"
#   include "hw/x86_64/ctx.h"
#   include "hw/x86_64/vmm.h"
#else 
#   error "Unknown architecture"
#endif

#include <navy/handover.h>

typedef Pml VmmSpace;

void hardware_init(Handover *handover);
void raise_debug(void);
void hlt(void);
void cli(void);
void sti(void);
VmmSpace space_create(void);