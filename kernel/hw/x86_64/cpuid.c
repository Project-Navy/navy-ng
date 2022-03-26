#include "cpuid.h"

CpuIDResult cpuid(uint32_t leaf, uint32_t subleaf)
{
    uint32_t cpuid_max;
    CpuID result;

    asm volatile ("cpuid"
        : "=a" (cpuid_max)
        : "a" (leaf & 0x80000000)
        : "rbx", "rcx", "rdx");

    if (leaf > cpuid_max)
    {
        return Err(CpuIDResult, cpuid_max);
    }

    asm volatile ("cpuid"
        : "=a" (result.eax), "=b" (result.ebx), "=c" (result.ecx), "=d" (result.edx)
        : "a" (leaf), "c" (subleaf));

    return Ok(CpuIDResult, result);
}