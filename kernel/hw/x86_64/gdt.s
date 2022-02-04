[ GLOBAL gdt_flush ]
gdt_flush:
    lgdt [rdi]
    push qword rbp
    mov rbp, rsp

    push qword 0x10
    push qword rbp
    pushf
    push qword qword 0x8
    push .trampoline
    iretq

.trampoline:
    pop rbp

    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ax, 0x28

    ret
 

[ GLOBAL tss_flush ]
tss_flush:
  mov ax, 0x28
  ltr ax
  ret