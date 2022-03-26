#pragma once 

#ifdef __x86_64__
#   include <hw/x86_64/ctx.h>
#endif

#include "arch.h"

#include <navy/range.h>
#include <navy/vec.h>

typedef enum 
{
    TASK_RUNNING,
    TASK_IDLE,
    TASK_BLOCKED,
    TASK_DEAD
} TaskState;

typedef struct 
{
    Context context;
    uintptr_t sp;
    Range stack;

    VmmSpace space;

    TaskState state;
    int return_value;
} Task;

Task *create_task(void);

typedef Vec(Task) TaskQueue;