#include "arch.h"
#include <stdint.h>

#include <navy/debug.h>

int64_t syscall_handler(Regs *regs)
{
    log$("Got syscall {}", regs->rax);
    return 0;
}