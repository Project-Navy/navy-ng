#include "arch.h"
#include "sched.h"
#include "task.h"

#include "hw/x86_64/pmm.h"
#include "hw/x86_64/vmm.h"

#include <navy/ipc.h>
#include <navy/shared_memory.h>
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

    if (msg->type == IPC_SHARED_MEMORY)
    {
        SharedMemory *mem = (SharedMemory *) msg->_addr;
        Range mem_range = {.base = msg->_addr, .length = sizeof(SharedMemory)};

        vmm_map_range(task->space, mem_range, mem_range, true);
        vmm_map_range(task->space, mem->range, mem->range, true);   
    }

    return 0;
}

int64_t syscall_getpid(MAYBE_UNUSED Regs *regs)
{
    return sched_current_pid();
}

int64_t syscall_create_shared_mem(Regs *regs)
{
    Task *task = sched_current_task();
    size_t size = regs->rbx;
    PmmOption buff = pmm_alloc(size);

    if (!buff.succ)
    {
        return 1;
    }

    SharedMemory *mem = (SharedMemory *) malloc(sizeof(SharedMemory));
    Range mem_range = (Range) {.base = (uintptr_t) mem, .length = sizeof(SharedMemory)};
    mem->range = UNWRAP(buff);
    mem->ref_count = 0;

    vmm_map_range(task->space, mem_range, mem_range, true);
    vmm_map_range(task->space, mem->range, mem->range, true);

    return (uintptr_t) mem;
}

Syscall syscall_matrix[] = {
    [SYS_LOG] = syscall_log,
    [SYS_EXIT] = syscall_exit,
    [SYS_IPC_SEND] = syscall_ipc_send,
    [SYS_IPC_RCV_SYNC] = syscall_ipc_rcv_sync,
    [SYS_GETPID] = syscall_getpid,
    [SYS_CREATE_SHARED_MEM] = syscall_create_shared_mem
};

int64_t syscall_handler(Regs *regs)
{
    return syscall_matrix[regs->rax](regs);
}