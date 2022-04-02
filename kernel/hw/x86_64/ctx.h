#pragma once 

#include <stdint.h>
#include "regs.h"

typedef struct 
{
    uintptr_t syscall_kernel_stack;
    uintptr_t syscall_user_stack;
    uintptr_t syscall_kernel_bstack;

    Regs regs;
    uint8_t simd[] __attribute__((aligned(64)));
} Context;

typedef struct 
{
    uint64_t arg1;
    uint64_t arg2;
    uint64_t arg3;
    uint64_t arg4;
    uint64_t arg5;
} TaskArgs;

void context_create(Context *ctx, uintptr_t ip, TaskArgs args);
void context_switch(Context *ctx, Regs *regs);
void context_save(Context *ctx, Regs *regs);
void context_load(Context *ctx, Regs *regs);