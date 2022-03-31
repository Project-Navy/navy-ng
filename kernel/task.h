#pragma once 

#ifdef __x86_64__
#   include "hw/x86_64/ctx.h"
#endif

#include "arch.h"

#include <navy/range.h>
#include <navy/vec.h>
#include <navy/result.h>
#include <brutal/str.h>
#include <sys/types.h>


typedef enum 
{
    TASK_RUNNING,
    TASK_IDLE,
    TASK_BLOCKED,
    TASK_DEAD
} TaskState;

typedef struct 
{
    Str name;
    uintptr_t sp;
    Range stack;

    VmmSpace space;

    TaskState state;
    int return_value;
    Context context;
} Task;


typedef Vec(Task *) TaskQueue;
typedef Option(Task *) TaskOption;

Task *create_task(Str name, PmlOption space);