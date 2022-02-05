#pragma once 

#include <navy/fmt.h>
#include <Uefi.h>

#ifdef __x86_64
#   define MMAP_KERNEL_BASE ((EFI_PHYSICAL_ADDRESS) 0xffffffff80000000)
#else
#   error "Unknown architecture"
#endif

#define gKernelAndModulesMemoryType ((EFI_MEMORY_TYPE) 0x80000000)
#define align_up$(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))


#define __FILENAME__                                                                               \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define log$(FORMAT, ...) \
	log_impl(__FILENAME__, __LINE__, FORMAT, PRINT_ARGS(__VA_ARGS__));

#define check_status$(status)                           \
    if (status != EFI_SUCCESS)                          \
    {                                                   \
        log$("Unsucessful EFI status: {x}", status);    \
        return status;                                  \
    }                                                   \

VOID log_impl(char const *filename, size_t line_nbr, char const *format, struct fmt_args args);
VOID cstr_stdout(char const *s);

VOID register_conout(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *self);
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut(void);

void register_bs(EFI_BOOT_SERVICES *self);
EFI_BOOT_SERVICES *Bs(void);
