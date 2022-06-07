#pragma once 

#ifdef __x86_64__
#   include "hw/x86_64/ctx.h"
#endif

#include "arch.h"

#include <navy/ipc.h>
#include <navy/range.h>
#include <navy/vec.h>
#include <brutal/result.h>
#include <brutal/str.h>
#include <unistd.h>

typedef Vec(Ipc) VecIpc;

typedef enum 
{
    TASK_RUNNING,
    TASK_IDLE,
    TASK_BLOCKED,
    TASK_DEAD,
    TASK_GONNADIE
} TaskState;

typedef struct 
{
    Str name;
    Range stack;

    VmmSpace space;

    TaskState state;
    int return_value;
    VecIpc mailbox;
    Context context;
} Task;

typedef Vec(Task *) TaskQueue;
typedef Option(Task *) TaskOption;

Task *create_task(Str name, PmlOption space);