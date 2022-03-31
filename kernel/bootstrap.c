#include "arch.h"
#include "elfloader.h"
#include "hw/x86_64/ctx.h"
#include "sched.h"

#include <stdlib.h>

#include <navy/debug.h> 
#include <navy/handover.h>

int _start(Handover *handover)
{
    handover_dump_mmap(handover);
    hardware_init(handover);
    sched_init();

    Module *test_module = unwrap_or_panic(handover_find_module(handover, str$("test")));
    Task *test = unwrap_or_panic(
        load_elf_module(
            test_module,
            (TaskArgs) {}
        )
    );

    sched_push_task(test);

    for(;;);
    return 0;
}