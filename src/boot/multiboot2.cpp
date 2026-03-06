#include <stdint.h>
#include <boot/multiboot2.h>
#include <utils.h>

size_t Multiboot2::total_memory_size = 0;
multiboot_tag_framebuffer *Multiboot2::framebuffer_info = nullptr;
multiboot_tag_mmap *Multiboot2::mmap_info = nullptr;

void Multiboot2::checkMagic(uintptr_t magic)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        return;
    }
}

void Multiboot2::parseMBI(uintptr_t mbi)
{
    multiboot_tag *tags = (multiboot_tag *)((uint8_t *)mbi + 8);
    while (tags->type != MULTIBOOT_TAG_TYPE_END)
    {
        if (tags->type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO)
        {
            multiboot_tag_basic_meminfo *tag = (multiboot_tag_basic_meminfo *)tags;
            total_memory_size = tag->mem_lower + tag->mem_upper;
        }
        if (tags->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
        {
            multiboot_tag_framebuffer *tag = (multiboot_tag_framebuffer *)tags;
            framebuffer_info = tag;
        }
        if (tags->type == MULTIBOOT_TAG_TYPE_MMAP)
        {
            multiboot_tag_mmap *tag = (multiboot_tag_mmap *)tags;
            mmap_info = tag;
        }
        tags = (multiboot_tag *)((uint8_t *)tags + align(tags->size, 8));
    }
}

void Multiboot2::init(uintptr_t magic, uintptr_t mbi)
{
    checkMagic(magic);
    parseMBI(mbi);
}

size_t Multiboot2::getTotalMemorySize()
{
    return total_memory_size;
}

multiboot_tag_mmap *Multiboot2::getMmapInfo()
{
    return mmap_info;
}