#pragma once 

#include <stdint.h>

static inline uint8_t asm_in8(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline void asm_out8(uint16_t port, uint8_t data)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline uintptr_t read_cr4(void)
{
    uintptr_t value;
    __asm__ volatile("mov %%cr4, %0" : "=r"(value));

    return value;
}

static inline uintptr_t read_cr3(void)
{
    uintptr_t value;
    __asm__ volatile("mov %%cr3, %0" : "=r"(value));

    return value;
}

static inline uintptr_t read_cr2(void)
{
    uintptr_t value;
    __asm__ volatile("mov %%cr2, %0" : "=r"(value));

    return value;
}

static inline uintptr_t read_cr0(void)
{
    uintptr_t value;
    __asm__ volatile("mov %%cr0, %0" : "=r"(value));

    return value;
}

static inline void write_cr4(uintptr_t val)
{
    asm volatile("mov %0, %%cr4" ::"a"(val));
}

static inline void write_cr3(uintptr_t val)
{
    asm volatile("mov %0, %%cr3" ::"a"(val));
}

static inline void write_cr0(uintptr_t val)
{
    asm volatile("mov %0, %%cr0" ::"a"(val));
}

// Lots of info here https://en.wikipedia.org/wiki/Control_register

enum cr0_bits 
{
    CR0_PE = 1,
    CR0_MP = (1 << 1),
    CR0_EM = (1 << 2),
    CR0_TS = (1 << 3),
    CR0_ET = (1 << 4),
    CR0_NE = (1 << 5),
    CR0_WP = (1 << 16),
    CR0_AM = (1 << 18),
    CR0_NW = (1 << 29),
    CR0_CD = (1 << 30),
    CR0_PG = (1 << 31)
};

enum cr4_bits 
{
    CR4_VME = 1,
    CR4_PVI = (1 << 1),
    CR4_TSD = (1 << 2),
    CR4_DE = (1 << 3),
    CR4_PSE = (1 << 4),
    CR4_PAE = (1 << 5),
    CR4_MCE = (1 << 6),
    CR4_PGE = (1 << 7),
    CR4_PCE = (1 << 8),
    CR4_OSFXSR = (1 << 9),
    CR4_OSXMMEXCPT = (1 << 10),
    CR4_UMIP = (1 << 11),
    CR4_LA57 = (1 << 12),
    CR4_VMXE = (1 << 13),
    CR4_SMXE = (1 << 14),
    CR4_FSGSBASE = (1 << 16),
    CR4_PCIDE = (1 << 17),
    CR4_OSXSAVE = (1 << 18),
    CR4_SMEP = (1 << 20),
    CR4_SMAP = (1 << 21),
    CR4_PKE = (1 << 22),
    CR4_CET = (1 << 23),
    CR4_PKS = (1 << 24)
};

enum xcr0_bits 
{
    XCR0_X87 = 1,
    XCR0_SSE = (1 << 1),
    XCR0_AVX = (1 << 2),
    XCR0_BNDREG = (1 << 3),
    XCR0_BNDCSR = (1 << 4),
    XCR0_OPMASK = (1 << 5),
    XCR0_ZMM_HI256 = (1 << 6),
    XCR0_HI16_ZMM = (1 << 7),
    XCR0_PKRU = (1 << 9),
    XCR0_CET_USER_STATE = (1 << 11),
    XCR0_CET_SUPERVISOR = (1 << 12)
};

static inline void write_xcr(uint32_t i, uint64_t value)
{
    asm volatile ("xsetbv"
        :
        : "a" ((uint32_t) value), "d" (value >> 32), "c" (i));
}

static inline void asm_xsave(uint8_t *region)
{
    asm volatile("xsave %0" ::"m"(*region), "a"(~(uintptr_t)0), "d"(~(uintptr_t)0)
                 : "memory");
}

static inline void asm_fxsave(void *region)
{
    asm volatile("fxsave (%0)" ::"a"(region));
}
