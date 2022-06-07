#include "ansi/unistd.h"
#include "arch.h"
#include "sched.h"
#include "task.h"
#include <stdint.h>
#include <unistd.h>
#include <navy/ipc.h>

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

int64_t syscall_ipc_send(Regs *regs)
{
    Ipc *msg = (Ipc *) regs->rbx;
    msg->sender = sched_current_pid();

    TaskOption optionReceiver = sched_get_by_pid(msg->receiver);

    if (!optionReceiver.succ)
    {
        return 1;   // TODO: Better error
    }
    
    Task *receiver = UNWRAP(optionReceiver);
    vec_push(&receiver->mailbox, *msg);

    return 0;
}

int64_t syscall_ipc_rcv_sync(Regs *regs)
{
    Task *task = sched_current_task();
    Ipc *msg = (Ipc *) regs->rbx;

    while (task->mailbox.length == 0)
    {
        sched_force_yield();
    }

    *msg = vec_pop(&task->mailbox);
    return 0;
}

int64_t syscall_getpid(MAYBE_UNUSED Regs *regs)
{
    return sched_current_pid();
}

Syscall syscall_matrix[] = {
    [SYS_LOG] = syscall_log,
    [SYS_EXIT] = syscall_exit,
    [SYS_IPC_SEND] = syscall_ipc_send,
    [SYS_IPC_RCV_SYNC] = syscall_ipc_rcv_sync,
    [SYS_GETPID] = syscall_getpid
};

int64_t syscall_handler(Regs *regs)
{
    return syscall_matrix[regs->rax](regs);
}