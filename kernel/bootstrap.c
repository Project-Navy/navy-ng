#include "arch.h"
#include "elfloader.h"
#include "sched.h"

#include <stdlib.h>

#include <navy/debug.h> 
#include <navy/handover.h>

int bootstrap(Handover *handover)
{
    handover_dump_mmap(handover);
    hardware_init(handover);
    sched_init();

    Module *test_module = UNWRAP_OR_PANIC(handover_find_module(handover, str$("test")), 
        "Couldn't retrieve the test module");
    
    Task *test = UNWRAP_OR_PANIC(
        load_elf_module(
            test_module,
            (TaskArgs) {}
        ),

        "Couldn't create a task with the test binary"
    );

    sched_push_task(test);

    for (;;)
    {
        sched_idle();
    }

    return 0;
}