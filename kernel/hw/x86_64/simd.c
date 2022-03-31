#include "simd.h"
#include "hw/x86_64/asm.h"
#include "hw/x86_64/cpuid.h"
#include "hw/x86_64/const.h"

#include <stdbool.h>

static __attribute__((aligned(PAGE_SIZE))) uint8_t initial_ctx[PAGE_SIZE] = {0};
static bool has_xsave = false;

void xcr0_init(bool has_avx, bool has_avx512)
{
    uint64_t xcr0 = 0;

    xcr0 |= XCR0_X87;
    xcr0 |= XCR0_SSE;

    if (has_avx)
    {
        xcr0 |= XCR0_AVX;
    }

    if (has_avx512)
    {
        xcr0 |= XCR0_OPMASK;
        xcr0 |= XCR0_ZMM_HI256;
        xcr0 |= XCR0_HI16_ZMM;
    }

    write_xcr(0, xcr0);
}

void simd_init(void)
{
    CpuIDResult features_result = cpuid(FEATURE_IDENTIFIERS_CPUID, 0);
    CpuID features = unwrap_or_panic(features_result);

    write_cr0(read_cr0() & ~CR0_EM);
    write_cr0(read_cr0() | CR0_MP);
    write_cr0(read_cr0() | CR0_NE);

    write_cr4(read_cr4() | CR4_OSFXSR);
    write_cr4(read_cr4() | CR4_OSXMMEXCPT);

    if (features.ecx & (1 << 26))
    {
        has_xsave = true;
        write_cr4(read_cr4() | CR4_OSXSAVE);
        xcr0_init(features.ecx & (1 << 28), features.ebx & (1 << 16));
    }

    asm volatile ("fninit");

    simd_ctx_save(initial_ctx);
}

void simd_ctx_save(void *ptr)
{
    if (has_xsave)
    {
        asm_xsave(ptr);
    }
    else
    {
        asm_fxsave(ptr);
    }
}

static size_t simd_context_size(void)
{
    if (has_xsave)
    {
        return unwrap_or_panic(cpuid(CPUID_PROC_EXTENDED_STATE_ENUMERATION, 0)).ecx;
    }
    else 
    {
        return 512;
    }
}

void simd_ctx_init(void *ptr)
{
    memcpy(ptr, initial_ctx, simd_context_size());
}

void simd_ctx_load(void *ptr)
{
    if (has_xsave)
    {
        asm_xrstor(ptr);
    }
    else  
    {
        asm_fxrstor(ptr);
    }
}