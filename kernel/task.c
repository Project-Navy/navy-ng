#include "task.h"
#include "arch.h"

#include <brutal/str.h>
#include <stdlib.h>

Task *create_task(Str name, PmlOption space)
{
    Task *self = (Task *) calloc(sizeof(Task), 1);
    self->state = TASK_RUNNING;
    self->name = name;
    self->stack = (Range) {
        .base = (uintptr_t) calloc(STACK_SIZE, 1),
        .length = STACK_SIZE
    };

    self->sp = self->stack.base + self->stack.length;

    if (space.success)
    {
        self->space = unwrap(space);
    }
    else  
    {
        self->space = unwrap_or_panic(vmm_create_space());
    }

    return self;
}