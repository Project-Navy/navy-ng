#include "arch.h"
#include "hw/x86_64/vmm.h"
#include "navy/option.h"
#include "sched.h"
#include "task.h"
#include <assert.h>
#include <navy/debug.h>
#include <navy/vec.h>

static TaskQueue tasks;
static DECLARE_LOCK(scheduler);
static bool scheduler_enabled = false;
static pid_t current_pid = 0;
static uint8_t tick = 0;

void sched_init(void)
{
    vec_init(&tasks);
    vec_push(&tasks, create_task(str$("Boot"), SOME(PmlOption, get_address_space())));
    scheduler_enabled = true;
}

bool sched_is_init(void)
{
    return scheduler_enabled;
}

static pid_t next_pid(void)
{
    if ((int) current_pid + 1 == (int) tasks.length)
    {
        current_pid = -1;
    }

    return ++current_pid;
}


void sched_yield(Regs *regs)
{
    LOCK(scheduler);

    if (++tick >= SWITCH_TICK)
    {
        tick = 0;

        Task *current_task = tasks.data[current_pid];
        context_save(&current_task->context, regs);

        while ((current_task = tasks.data[next_pid()])->state != TASK_RUNNING);

        context_switch(&current_task->context, regs);
        vmm_switch_space(current_task->space);
    }

    UNLOCK(scheduler);
}

void sched_force_yield() 
{
    tick = SWITCH_TICK;
    sti();
}

bool is_scheduler_enabled(void)
{
    return scheduler_enabled;
}

void sched_push_task(Task *task)
{
    LOCK(scheduler);

    vec_push(&tasks, task);

    UNLOCK(scheduler);
}

Task *sched_current_task(void)
{
    return tasks.data[current_pid];
}

pid_t sched_current_pid(void)
{
    return current_pid;
}

void sched_idle(void)
{
    for (size_t i = 0; i < tasks.length; i++)
    {
        Task *task = tasks.data[i];
        
        LOCK(scheduler);

        if (task->state == TASK_DEAD && task->context.syscall_kernel_bstack != 0)
        {
            free((void *) task->context.syscall_kernel_bstack);
            task->context.syscall_kernel_bstack = 0;
        }

        UNLOCK(scheduler);
    }

}