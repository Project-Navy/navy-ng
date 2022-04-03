#include "task.h"
#include "arch.h"

#include <brutal/result.h>
#include <brutal/str.h>
#include <stdlib.h>

Task *create_task(Str name, PmlOption space)
{
    Task *self = (Task *) calloc(sizeof(Task), 1);
    self->state = TASK_RUNNING;
    self->name = name;
    self->stack = UNWRAP_OR_PANIC(pmm_alloc(STACK_SIZE), "Out of memory");

    if (space.ok)
    {
        self->space = UNWRAP(space);
    }
    else  
    {
        self->space = UNWRAP_OR_PANIC(vmm_create_space(), "Couldn't create an address space");
    }

    vmm_map_range(self->space, (Range) {
        .base = USER_STACK_BASE,
        .length = STACK_SIZE 
    }, self->stack, true);

    return self;
}