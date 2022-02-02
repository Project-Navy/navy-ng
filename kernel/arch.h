#pragma once 

#ifdef __x86_64__
#   include "hw/x86_64/com.h"
#else 
#   error "Unknown architecture"
#endif