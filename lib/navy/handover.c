#include <navy/handover.h>

void handover_dump_mmap(Handover *handover)
{
    for (size_t i = 0; i < handover->memmap_count; i++)
    {
        log$("{a} - {a} ({})", handover->memmaps[i].range.base, handover->memmaps[i].range.length, 
            mmap_type_str[handover->memmaps[i].type]);
    }
}

ModuleOption handover_find_module(Handover *handover, Str name)
{
    for (size_t i = 0; i < handover->module_count; i++)
    {
        if (str_eq(handover->modules[i].name, name))
        {
            return Some(ModuleOption, &handover->modules[i]);
        }
    }

    return None(ModuleOption);
}