#include <assert.h>
#include <navy/task.h>
#include <navy/debug.h>

static TaskQueue tasks;

void sched_init(void)
{
    vec_init(&tasks);
}