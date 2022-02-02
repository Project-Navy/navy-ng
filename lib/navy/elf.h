#pragma once 

#include <stdint.h>

#define EI_NIDENT 16

enum E_IDENT_INDEX 
{
    EI_MAG0,
    EI_MAG1,
    EI_MAG2,
    EI_MAG3,
    EI_CLASS,
    EI_DATA,
    EI_VERSION,
    EI_OSABI,
    EI_ABIVERSION,
    EI_PAD
};

enum P_TYPES 
{
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
    PT_TLS,
    PT_LOOS = 0x60000000,
    PT_HIOS = 0x6FFFFFFF,
    PT_LOPROC = 0x70000000,
    PT_HIPROC = 0x7FFFFFFF
};

typedef struct
{
    uint8_t  e_ident[EI_NIDENT];  
    uint16_t e_type;       
    uint16_t e_machine;    
    uint32_t e_version;    
    uint64_t e_entry;      
    uint64_t e_phoff;      
    uint64_t e_shoff;      
    uint32_t e_flags;      
    uint16_t e_ehsize;     
    uint16_t e_phentsize;  
    uint16_t e_phnum;      
    uint16_t e_shentsize;  
    uint16_t e_shnum;      
    uint16_t e_shstrndx;   
} Elf64_Ehdr;

typedef struct
{
    uint32_t p_type;      
    uint32_t p_flags;     
    uint64_t p_offset;    
    uint64_t p_vaddr;     
    uint64_t p_paddr;     
    uint64_t p_filesz;    
    uint64_t p_memsz;     
    uint64_t p_align;     
} Elf64_Phdr;


typedef struct 
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize; 
} Elf64_Shdr;