#include <x86_64/interrupts/exceptions.h>
#include <stdint.h>

struct IDTEntry
{
    uint16_t offset_1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attributes;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
} __attribute__((packed));

struct IDTPtr
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void idt_init();
void idt_set_entry(uint8_t _vector, uint64_t handler, uint8_t type_attr);
void exceptions_init();

void panic(const char *title, InterruptFrame *frame, uint64_t err_code);
void panic(const char *title, InterruptFrame *frame, uint64_t cr2, uint64_t err_code);