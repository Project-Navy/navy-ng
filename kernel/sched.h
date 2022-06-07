#pragma once 

#include "task.h"
#include <stdbool.h>
#include <navy/option.h>

#define SWITCH_TICK 8

void sched_init(void);
bool is_scheduler_enabled(void);
void sched_push_task(Task *task);
void sched_yield(Regs *regs);
Task *sched_current_task(void);
TaskOption sched_get_by_pid(pid_t pid);
pid_t sched_current_pid(void);
void sched_force_yield(void);
void sched_idle(void);