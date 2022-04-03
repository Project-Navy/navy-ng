#include "arch.h"
#include "elfloader.h"
#include "hw/x86_64/ctx.h"
#include "task.h"

#include <navy/lock.h>
#include <navy/debug.h>
#include <navy/elf.h>
#include <navy/option.h>
#include <brutal/result.h>

static DECLARE_LOCK(elfloader);

TaskOption load_elf_module(Module *m, TaskArgs args)
{
    LOCK(elfloader);


    Elf64_Ehdr *header = (Elf64_Ehdr *) m->addr.base;

    if(header->e_ident[EI_MAG0] != 0x7f || header->e_ident[EI_MAG1] != 'E' || 
        header->e_ident[EI_MAG2] != 'L' || header->e_ident[EI_MAG3] != 'F')
    {
        return NONE(TaskOption);
    }

    log$("Loading {} ({a} - {a})", m->name, m->addr.base, m->addr.base + m->addr.length);
    Elf64_Phdr *phdr = (Elf64_Phdr *) m->addr.base + header->e_phoff;
    PmlOption space_option = vmm_create_space();

    if (!space_option.ok)
    {
        return NONE(TaskOption);
    }
    
    VmmSpace space = UNWRAP(space_option);

    for (size_t i = 0; i < header->e_phnum; i++)
    {
        if (phdr->p_type == PT_LOAD)
        {
            Range addr = UNWRAP_OR_PANIC(pmm_alloc(ALIGN_UP(phdr->p_memsz, PAGE_SIZE)), "Out of memory");
            vmm_map_range(space, (Range) {
                .base = phdr->p_vaddr,
                .length = addr.length
            }, addr, true);

            memcpy((void *) mmap_phys_to_kernel(addr.base), (void *) (m->addr.base + phdr->p_offset), phdr->p_filesz);
            memcpy((void *) (mmap_phys_to_kernel(addr.base) + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);
        }

        phdr = (Elf64_Phdr *) (m->addr.base + header->e_phoff + (i * header->e_phentsize));
    }

    Task *task = create_task(str$(m->name), SOME(PmlOption, space));
    context_create(&task->context, header->e_entry, args);

    UNLOCK(elfloader);

    return SOME(TaskOption, task);
}