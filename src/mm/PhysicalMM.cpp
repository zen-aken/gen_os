#include <stdint.h>
#include <stddef.h>
#include <mm/PhysicalMM.h>
#include <boot/multiboot2.h>

size_t PhysicalMM::memory_size;
multiboot_tag_mmap *PhysicalMM::mmap;
multiboot_mmap_entry *PhysicalMM::availabe_memory_chunk;
uintptr_t *PhysicalMM::pages;
uint32_t PhysicalMM::page_count;

void PhysicalMM::setTotalMemorySize(size_t _memory_size)
{
    memory_size = _memory_size;
}

void PhysicalMM::setMmap(multiboot_tag_mmap *_mmap)
{
    mmap = _mmap;
}

void PhysicalMM::setBiggestUsableMemoryChunk()
{
    multiboot_mmap_entry *entry = mmap->entries;
    multiboot_mmap_entry *current_entry = entry;
    while ((uint8_t *)entry < (uint8_t *)mmap->entries + mmap->size)
    {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE && entry->len > current_entry->len)
        {
            current_entry = entry;
        }

        entry = (multiboot_mmap_entry *)((uint8_t *)entry + mmap->entry_size);
    }
    if (current_entry->type != MULTIBOOT_MEMORY_AVAILABLE)
    {
        return; //! ADD ERROR HANDLING
    }
    availabe_memory_chunk = current_entry;
}

void PhysicalMM::setPageSize()
{
    page_count = availabe_memory_chunk->len / 4096;
}

uintptr_t PhysicalMM::allocMem(size_t index)
{
    size_t page = index / 64;
    size_t bit = index % 64;
    pages[page] |= (1ULL << bit);
    return availabe_memory_chunk->addr + index * 4096;
}

void PhysicalMM::freeMem(size_t index)
{
    size_t page = index / 64;
    size_t bit = index % 64;
    pages[page] &= ~(1ULL << bit);
}

bool PhysicalMM::isPageFree(size_t index)
{
    size_t page = index / 64;
    size_t bit = index % 64;
    if (pages[page] & (1ULL << bit))
    {
        return false;
    }
    else
    {
        return true;
    }
}

uintptr_t PhysicalMM::allocFreeMem()
{
    for (size_t i = 0; i < page_count; i++)
    {
        if (isPageFree(i))
        {
            return allocMem(i);
        }
    }
    return 0; // out of memory
}

void PhysicalMM::init()
{
    memory_size = 0;
    mmap = nullptr;
    availabe_memory_chunk = nullptr;
    pages = nullptr;
    page_count = 0;

    setTotalMemorySize(Multiboot2::getTotalMemorySize());
    setMmap(Multiboot2::getMmapInfo());
    setBiggestUsableMemoryChunk();
    setPageSize();

    // set pages start address
    pages = (uintptr_t *)availabe_memory_chunk->addr;

    uint32_t page = 0;
    uint32_t bit = 0;

    // reset pages
    for (size_t i = 0; i < page_count / 64; i++)
    {
        pages[i] = 0;
    }

    size_t bitmap_pages = (page_count / 8 + 4095) / 4096;
    for (int i = 0; i < bitmap_pages; i++)
    {
        allocMem(i);
    }
}
