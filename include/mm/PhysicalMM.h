#include <stdint.h>
#include <stddef.h>
#include <boot/multiboot2.h>

class PhysicalMM
{
private:
    static size_t memory_size;
    static multiboot_tag_mmap *mmap;
    static multiboot_mmap_entry *availabe_memory_chunk;
    static uintptr_t *pages;
    static uint32_t page_count;

public:
    static void setTotalMemorySize(size_t _memory_size);
    static void setMmap(multiboot_tag_mmap *_mmap);
    static void setPageSize();
    static void setBiggestUsableMemoryChunk();
    static void init();
    static uintptr_t allocMem(size_t index);
    static void freeMem(size_t index);
    static bool isPageFree(size_t index);
    static uintptr_t allocFreeMem();
};
