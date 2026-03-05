#include <boot/multiboot2.h>
#include <mm/physical_memory.h>
#include <drivers/framebuffer.h>
#include <utils.h>

void Multiboot2::check_magic(uintptr_t magic)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        return;
    }
}

void Multiboot2::parse_mbi(uintptr_t mbi)
{
    multiboot_tag *tags = (multiboot_tag *)((uint8_t *)mbi + 8);
    while (tags->type != MULTIBOOT_TAG_TYPE_END)
    {
        if (tags->type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO)
        {
            multiboot_tag_basic_meminfo *tag = (multiboot_tag_basic_meminfo *)tags;
            Physical_Memory::set_total_memory_size(tag->mem_lower + tag->mem_upper);
        }
        if (tags->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
        {
            multiboot_tag_framebuffer *tag = (multiboot_tag_framebuffer *)tags;
            Framebuffer::set_framebuffer_info(tag);
        }
        if (tags->type == MULTIBOOT_TAG_TYPE_MMAP)
        {
            multiboot_tag_mmap *tag = (multiboot_tag_mmap *)tags;
            Physical_Memory::set_mmap(tag);
        }
        tags = (multiboot_tag *)((uint8_t *)tags + align(tags->size, 8));
    }
}

void Multiboot2::multiboot2_init(uintptr_t magic, uintptr_t mbi)
{
    check_magic(magic);
    parse_mbi(mbi);
}