#include <stdint.h>
#include <stddef.h>
#include <boot/multiboot2.h>

class Physical_Memory
{
private:
    static size_t memory_size;
    static multiboot_tag_mmap *mmap;

public:
    static void set_total_memory_size(size_t _memory_size);
    static void set_mmap(multiboot_tag_mmap *_mmap);
};
