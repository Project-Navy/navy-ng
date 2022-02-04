#include "arch.h"
#include "hw/x86_64/rtc.h"

#include <hw/x86_64/regs.h>
#include <hw/x86_64/asm.h>

#include <navy/fmt.h>

#include <stdlib.h>

[[maybe_unused]] static char *_exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Detected Overflow",
    "Out Of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad Tss",
    "Segment Not Present",
    "StackFault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};


static const char *comments[] = {
    "Panic At The Kernel !",
    "Press F to pay respect.",
    "Could not boot up kernel because no",
    "Booting up is not supported on this kernel",
    "Seems like Duolingo halted the kernel",
    "Hey /r/softwaregore",
    "Believe me, everything is fine :)",
    "The kernel defined that 1 + 1 = 3",
    "Congratulations, this kernel can now run Half Life 3 !",
    "Still more stable than Windows",
    "Looks like the CPU took a lil' nap",
    "Stability is bloat",
    "The kernel took some vacations",
    "https://www.youtube.com/watch?v=dQw4w9WgXcQ",
    "Still faster developpement than GNU Hurd",
    "Did you try to rewrite the kernel in rust ?",
    "Better of doing a megalithicc kernel",
    "Keyboard Slayer, just contribute to Brutal wtf",
    "The dev of this kernel is the worst at copying and pasting",
    "Maybe if you create your own cpu architecture it will work",
    "DevseChan, what if we kissed during the kernel panic",
    "\033[5mPaging hell is real\033[0m",
    "",
};

void exception_handler(Regs *regs)
{
    srandom(rtc_sec() * rtc_sec());
    int funny_id = random() % (sizeof(comments) / sizeof(comments[0]));

    print_format(serial_puts, "\033[31m!!\033[33m-----------------------------------------------------------------------------------\033[0m\n");
    print_format(serial_puts, "\n\tKERNEL PANIC\n\t\033[51m{}\033[0m\n\t", comments[funny_id]);
    print_format(serial_puts, "Exception {} (0x{x}) Err: {d}\n\n", _exception_messages[regs->int_no], regs->int_no, regs->error_code);
    print_format(serial_puts, "RAX {a} RBX {a} RCX {a} RDX {a}\n", regs->rax,
                 regs->rbx, regs->rcx, regs->rdx);
    print_format(serial_puts, "RSI {a} RDI {a} RBP {a} RSP {a}\n", regs->rsi,
                 regs->rdi, regs->rbp, regs->rsp);
    print_format(serial_puts, " R8 {a}  R9 {a} R10 {a} R11 {a}\n", regs->r8,
                 regs->r9, regs->r10, regs->r11);
    print_format(serial_puts, "R12 {a} R13 {a} R14 {a} R15 {a}\n", regs->r12,
                 regs->r13, regs->r14, regs->r15);
    print_format(serial_puts, "CR2 {a} CR3 {a} RIP \033[7m{a}\033[0m\n", read_cr2(), read_cr3(), regs->rip);
    print_format(serial_puts, "\033[33m-----------------------------------------------------------------------------------\033[31m!!\033[0m\n");
}

uint64_t interrupts_handler(uint64_t rsp)
{
    Regs *regs = (void *) rsp;

    if (regs->int_no < 32)
    {
        exception_handler(regs);

        for(;;)
        {
            cli();
            hlt();
        }
    }

    return rsp;
}