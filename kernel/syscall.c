#include "arch.h"
#include "sched.h"
#include "task.h"
#include <stdint.h>
#include <unistd.h>

typedef int64_t (*Syscall)(Regs *);

int64_t syscall_log(Regs *regs)
{
    serial_puts((char const *) regs->rbx);
    return 0;
}

int64_t syscall_exit(Regs *regs)
{
    Task *current_task = sched_current_task();

    current_task->return_value = regs->rbx;
    current_task->state = TASK_DEAD;

    log$("Task {} exited with exit code {}", current_task->name, current_task->return_value);

    sched_force_yield();
    for(;;);
}

Syscall syscall_matrix[] = {
    [SYS_LOG] = syscall_log,
    [SYS_EXIT] = syscall_exit
};

int64_t syscall_handler(Regs *regs)
{
    regs->rax = syscall_matrix[regs->rax](regs);
    return 0;
}