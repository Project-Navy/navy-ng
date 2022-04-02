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

    if (space.ok)
    {
        self->space = UNWRAP(space);
    }
    else  
    {
        self->space = UNWRAP_OR_PANIC(vmm_create_space(), "Couldn't create an address space");
    }

    return self;
}