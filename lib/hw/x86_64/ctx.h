#pragma once 

#include <stdint.h>
#include "regs.h"

typedef struct 
{
    uintptr_t syscall_kernel_stack;
    uintptr_t syscall_user_stack;

    Regs regs;
} Context;