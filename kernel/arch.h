#pragma once 

#ifdef __x86_64__
#   include "hw/x86_64/com.h"
#   include "hw/x86_64/const.h"
#else 
#   error "Unknown architecture"
#endif

#include <navy/handover.h>

void hardware_init(Handover *handover);
void raise_debug(void);
void hlt(void);
void cli(void);
void sti(void);