#include <stdint.h>
#include <boot/multiboot2.h>
#include <mm/PhysicalMM.h>

extern "C" void kernel_main(uintptr_t magic, uintptr_t mbi)
{
    Multiboot2::init(magic, mbi);
    PhysicalMM::init();
    while (1)
    {
        asm volatile("hlt");
    }
}