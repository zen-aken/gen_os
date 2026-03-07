#pragma once
#include <stdint.h>
#include <stddef.h>
#include <limine.h>

class PhysicalMM
{
public:
    static void init(limine_memmap_response *memmap);
    static uintptr_t allocFreeMem();
    static void freeMem(size_t index);
    static bool isPageFree(size_t index);

private:
    static uintptr_t allocMem(size_t index);
    static void setBiggestUsableMemoryChunk(limine_memmap_response *memmap);
    static void setPageSize();

    static limine_memmap_entry *available_memory_chunk;
    static uintptr_t *pages;
    static uint32_t page_count;
};