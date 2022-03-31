#pragma once 

#include "task.h"
#include <stdbool.h>

#define SWITCH_TICK 8

void sched_init(void);
bool is_scheduler_enabled(void);
void sched_push_task(Task *task);
void sched_yield(Regs *regs);