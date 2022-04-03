#include "arch.h"
#include "hw/x86_64/vmm.h"
#include "sched.h"
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

static pid_t next_pid(void)
{
    if (current_pid + 1 == (int) tasks.length)
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
        
        //log$("Switching to {}", current_task->name);

        context_switch(&current_task->context, regs);
        vmm_switch_space(current_task->space);
    }

    UNLOCK(scheduler);
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