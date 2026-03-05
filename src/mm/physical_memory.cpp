#include <stdint.h>
#include <stddef.h>
#include <mm/physical_memory.h>

uint64_t Physical_Memory::memory_size = 0;
multiboot_tag_mmap *Physical_Memory::mmap = nullptr;

void Physical_Memory::set_total_memory_size(size_t _memory_size)
{
    memory_size = _memory_size;
}