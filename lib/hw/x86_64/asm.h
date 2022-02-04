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

static inline void load_cr3(uintptr_t val)
{
    asm volatile("mov %0, %%cr3" ::"a"(val));
}