#pragma once 

#ifdef __x86_64__
#   include "hw/x86_64/com.h"
#else 
#   error "Unknown architecture"
#endif

void hardware_init(void);
void raise_debug(void);
void hlt(void);
void cli(void);
void sti(void);