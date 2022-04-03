#include "arch.h"
#include <stdint.h>

#include <navy/debug.h>

int64_t syscall_handler(Regs *regs)
{
    (void) regs;
    serial_puts("Hello Syscall world !\n");
    return 0;
}