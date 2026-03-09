#include <mm/PhysicalMM.h>
#include <utils/io.h>
#include <utils.h>

limine_memmap_entry *PhysicalMM::available_memory_chunk = 0;
uintptr_t *PhysicalMM::pages = 0;
uint32_t PhysicalMM::page_count = 0;
limine_memmap_response *PhysicalMM::_memmap = 0;

bool PhysicalMM::isPageFree(size_t index)
{
    uint32_t page = index / 64;
    uint32_t bit = index % 64;
    if (pages[page] & (1ULL << bit))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void PhysicalMM::freeMem(size_t index)
{
    uint32_t page = index / 64;
    uint32_t bit = index % 64;
    pages[page] &= ~(1ULL << bit);
}

uintptr_t PhysicalMM::allocFreeMem()
{
    for (size_t index = 0; index < page_count; index++)
    {
        if (isPageFree(index))
        {
            return allocMem(index);
        }
    }
    return 0; // out of memory
}

uintptr_t PhysicalMM::allocMem(size_t index)
{
    uint32_t page = index / 64;
    uint32_t bit = index % 64;
    pages[page] |= (1ULL << bit);
    return available_memory_chunk->base + index * 4096;
}

void PhysicalMM::setBiggestUsableMemoryChunk(limine_memmap_response *memmap)
{
    limine_memmap_entry **entry = memmap->entries;
    limine_memmap_entry *currentChunk = entry[0];
    log(LogType::INFO, false, "PMM", "Usable memory:\n");
    for (size_t index = 0; index < memmap->entry_count; index++)
    {
        if (entry[index]->type == LIMINE_MEMMAP_USABLE)
        {
            print("[ INFO ] PMM ==> Memory chunk found! size: %x - %x\n", entry[index]->base, entry[index]->base + entry[index]->length);
        }
        if (entry[index]->length > currentChunk->length && entry[index]->type == LIMINE_MEMMAP_USABLE)
        {
            currentChunk = entry[index];
        }
    }
    if (currentChunk->type != LIMINE_MEMMAP_USABLE)
    {
        log(LogType::ERR, false, "PMM", "No memory chunk available!");
        return;
    }
    available_memory_chunk = currentChunk;
}

void PhysicalMM::setPageSize()
{
    page_count = (available_memory_chunk->length + 4095) / 4096;
    print("[ INFO ] PMM ==> Page count: %d\n", page_count);
}

void PhysicalMM::init(limine_memmap_response *memmap)
{
    _memmap = memmap;
    setBiggestUsableMemoryChunk(memmap);
    setPageSize();

    // set pages address
    pages = (uintptr_t *)available_memory_chunk->base;

    // reset pages
    for (size_t i = 0; i < page_count / 64; i++)
    {
        pages[i] = 0;
    }
    log(LogType::INFO, false, "PMM", "Page's reseted\n");

    // map bitmap
    size_t bitmap_pages = (page_count / 8 + 4095) / 4096;
    for (size_t i = 0; i < bitmap_pages; i++)
    {
        allocMem(i);
    }
}

limine_memmap_response *PhysicalMM::getMemmap()
{
    return _memmap;
}