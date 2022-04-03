section .bss
align 16

global __stack_bottom
global __stack_top

__stack_bottom:
resb 16384 ; 16 KiB
__stack_top:

section .text

global _kstart:function (_kstart.end - _kstart)
_kstart:
    cli
	cld

    mov rsp, __stack_top
	xor rbp, rbp

    extern brutal_entry
    call brutal_entry

.hang:
	cli
	hlt
	jmp .hang
.end: