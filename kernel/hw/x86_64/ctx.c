#include "ctx.h"
#include "simd.h"
#include "asm.h"

#include <navy/lock.h>

static DECLARE_LOCK(ctx);

void context_create(Context *ctx, uintptr_t ip, uintptr_t sp, TaskArgs args)
{
    LOCK(ctx);

    Regs regs = {};

    regs.cs = 0x23;
    regs.ss = 0x1b;
    regs.rip = ip;
    regs.rsp = sp;

    regs.rdi = args.arg1;
    regs.rsi = args.arg2;
    regs.rdx = args.arg3;
    regs.rcx = args.arg4;
    regs.r8 = args.arg5;

    ctx->regs = regs;

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