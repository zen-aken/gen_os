#include <x86_64/interrupts/exceptions.h>
#include <x86_64/interrupts/idt.h>
#include <stdint.h>
#include <utils/io.h>

//* #00 Division Error
__attribute__((interrupt, target("general-regs-only"))) void isr0(InterruptFrame *frame, uint64_t error_code)
{
    panic("Division error", frame, error_code);
}

//? #01 Debug [required for costum compiler]
__attribute__((interrupt, target("general-regs-only"))) void isr1(InterruptFrame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
}

//* #02 NMI
__attribute__((interrupt, target("general-regs-only"))) void isr2(InterruptFrame *frame, uint64_t error_code)
{
    panic("Non-Maskable Interrupt", frame, error_code);
}

//? #03 Breakpoint [required for costum compiler]
__attribute__((interrupt, target("general-regs-only"))) void isr3(InterruptFrame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
}

//! #04 Overflow [UNUSED]
__attribute__((interrupt, target("general-regs-only"))) void isr4(InterruptFrame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
}

//! #05 Bound Range Exceeded [UNUSED]
__attribute__((interrupt, target("general-regs-only"))) void isr5(InterruptFrame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
}

//* #06 Invalid Opcode
__attribute__((interrupt, target("general-regs-only"))) void isr6(InterruptFrame *frame, uint64_t error_code)
{
    panic("Invalid Opcode", frame, error_code);
}

//* #07 Device Not Available
__attribute__((interrupt, target("general-regs-only"))) void isr7(InterruptFrame *frame, uint64_t error_code)
{
    panic("Device Not Avaible", frame, error_code);
}

//* #08 Double Fault
__attribute__((interrupt, target("general-regs-only"))) void isr8(InterruptFrame *frame, uint64_t error_code)
{
    panic("Double Fault", frame, error_code);
}

//! #09 [RESERVED]
__attribute__((interrupt, target("general-regs-only"))) void isr9(InterruptFrame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
}

//* #10 Invalid TSS
__attribute__((interrupt, target("general-regs-only"))) void isr10(InterruptFrame *frame, uint64_t error_code)
{
    panic("Invalid TSS", frame, error_code);
}

//* #11 Segment Not Present
__attribute__((interrupt, target("general-regs-only"))) void isr11(InterruptFrame *frame, uint64_t error_code)
{
    panic("Segment Not Present", frame, error_code);
}

//* #12 Stack-Segment Fault
__attribute__((interrupt, target("general-regs-only"))) void isr12(InterruptFrame *frame, uint64_t error_code)
{
    panic("Stack-Segment Fault", frame, error_code);
}

//* #13 General Protection
__attribute__((interrupt, target("general-regs-only"))) void isr13(InterruptFrame *frame, uint64_t error_code)
{
    panic("General Protection", frame, error_code);
}

//* #14 Page Fault
__attribute__((target("general-regs-only"), interrupt)) void isr14(InterruptFrame *frame, uint64_t error_code)
{
    uint64_t cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2));
    panic("Page fault", frame, cr2, error_code);
}

//! #15 [RESERVED]
__attribute__((interrupt, target("general-regs-only"))) void isr15(InterruptFrame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
}