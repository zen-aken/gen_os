#include <stdint.h>

extern "C" void kernel_main(uintptr_t magic, uintptr_t mbi)
{

    while (1)
    {
        asm volatile("hlt");
    }
}