#include "ctx.h"
#include "simd.h"
#include "asm.h"
#include "gdt.h"

#include <navy/lock.h>
#include <stdlib.h>

static DECLARE_LOCK(ctx);

void context_create(Context *ctx, uintptr_t ip, TaskArgs args)
{
    LOCK(ctx);

    Regs regs = {};

    regs.cs = (GDT_USER_CODE * 8) | 3;
    regs.ss = (GDT_USER_DATA * 8) | 3;
    regs.rip = ip;
    regs.rsp = USER_STACK_BASE + STACK_SIZE;
    regs.rbp = USER_STACK_BASE;
    regs.rflags = 0x202;

    regs.rdi = args.arg1;
    regs.rsi = args.arg2;
    regs.rdx = args.arg3;
    regs.rcx = args.arg4;
    regs.r8 = args.arg5;

    ctx->regs = regs;

    ctx->syscall_kernel_bstack = (uintptr_t) malloc(STACK_SIZE);
    ctx->syscall_kernel_stack = ctx->syscall_kernel_bstack + STACK_SIZE;

    simd_ctx_init(ctx->simd);

    UNLOCK(ctx);
}

void context_save(Context *ctx, Regs *regs)
{
    simd_ctx_save(ctx->simd);
    ctx->regs = *regs;
}

void context_switch(Context *ctx, Regs *regs)
{
    asm_write_msr(MSR_GS_BASE, (uintptr_t) ctx);
    asm_write_msr(MSR_KERN_GS_BASE, (uintptr_t) ctx);

    *regs = ctx->regs;
    simd_ctx_load(ctx->simd);
}