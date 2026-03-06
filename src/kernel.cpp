#include <stdint.h>
#include <boot/multiboot2.h>

extern "C" void kernel_main(uintptr_t magic, uintptr_t mbi)
{
    Multiboot2::init(magic, mbi);
    while (1)
    {
        asm volatile("hlt");
    }
}