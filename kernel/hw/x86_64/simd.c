#include "simd.h"
#include "hw/x86_64/cpuid.h"

void simd_init(void)
{
    CpuIDResult features_result = cpuid(FEATURE_IDENTIFIERS_CPUID, 0);
    [[maybe_unused]] CpuID features = unwrap_or_panic(features_result);
    log$("OK");
}