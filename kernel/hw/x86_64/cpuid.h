#pragma once

#include <stdint.h>
#include <brutal/result.h>

#define FEATURE_IDENTIFIERS_CPUID (1)
#define CPUID_PROC_EXTENDED_STATE_ENUMERATION (13)

typedef struct 
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} CpuID;

typedef Result(CpuID, uint32_t) CpuIDResult;
CpuIDResult cpuid(uint32_t leaf, uint32_t subleaf);