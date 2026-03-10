#include <x86_64/interrupts/idt.h>
#include <x86_64/interrupts/exceptions.h>
#include <utils/io.h>
#include <stdint.h>

IDTEntry idt[256];
IDTPtr idt_ptr = {};

void idt_set_entry(uint8_t _vector, uint64_t handler, uint8_t type_attr)
{
    IDTEntry entry;
    entry.offset_1 = handler & 0xFFFF;
    entry.offset_2 = (handler >> 16) & 0xFFFF;
    entry.offset_3 = (handler >> 32) & 0xFFFFFFFF;
    entry.selector = 0x28;
    entry.ist = 0;
    entry.type_attributes = type_attr;
    entry.zero = 0;
    idt[_vector] = entry;
}

void exceptions_init()
{
    idt_set_entry(0, (uint64_t)isr0, 0x8E);
    idt_set_entry(1, (uint64_t)isr1, 0x8E);
    idt_set_entry(2, (uint64_t)isr2, 0x8E);
    idt_set_entry(3, (uint64_t)isr3, 0x8E);
    idt_set_entry(4, (uint64_t)isr4, 0x8E);
    idt_set_entry(5, (uint64_t)isr5, 0x8E);
    idt_set_entry(6, (uint64_t)isr6, 0x8E);
    idt_set_entry(7, (uint64_t)isr7, 0x8E);
    idt_set_entry(8, (uint64_t)isr8, 0x8E);
    // 9 reserved
    idt_set_entry(10, (uint64_t)isr10, 0x8E);
    idt_set_entry(11, (uint64_t)isr11, 0x8E);
    idt_set_entry(12, (uint64_t)isr12, 0x8E);
    idt_set_entry(13, (uint64_t)isr13, 0x8E);
    idt_set_entry(14, (uint64_t)isr14, 0x8E);
    // 15 reserved
}

void interrupts_init() {}

void panic(const char *title, InterruptFrame *frame, uint64_t err_code)
{
    print("%c[KERNEL PANIC] %s\n", Colors::Red, title);
    print("%cRIP: %x\n", Colors::Red, frame->rip);
    print("%cRSP: %x\n", Colors::Red, frame->cs);
    print("%cRSP: %x\n", Colors::Red, frame->rflags);
    print("%cRSP: %x\n", Colors::Red, frame->rsp);
    print("%cRSP: %x\n", Colors::Red, frame->ss);
    print("%cERR: %x\n", Colors::Red, err_code);
    while (1)
    {
        asm volatile("hlt");
    }
}

//* Page fault specific panic
void panic(const char *title, InterruptFrame *frame, uint64_t cr2, uint64_t err_code)
{
    print("%c[KERNEL PANIC] %s\n", Colors::Red, title);
    print("%cRIP: %x\n", Colors::Red, frame->rip);
    print("%cRSP: %x\n", Colors::Red, frame->rsp);
    print("%cCR2: %x\n", Colors::Red, cr2);
    print("%cERR: %x\n", Colors::Red, err_code);
    while (1)
    {
        asm volatile("hlt");
    }
}

void idt_init()
{
    log(LogType::INFO, false, "IDT", "Initializing IDT\n");
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint64_t)&idt;
    exceptions_init();
    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
    log(LogType::INFO, false, "IDT", "IDT Initialized\n");
}