#pragma once 

#include "task.h"
#include <stdbool.h>

#define SWITCH_TICK 8

void sched_init(void);
bool is_scheduler_enabled(void);
void sched_push_task(Task *task);
void sched_yield(Regs *regs);
Task *sched_current_task(void);
pid_t sched_current_pid(void);
void sched_force_yield(void);
void sched_idle(void);