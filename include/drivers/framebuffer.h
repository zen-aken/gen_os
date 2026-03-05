#include <multiboot2.h>

class Framebuffer
{
private:
    static multiboot_tag_framebuffer *framebuffer_info;

public:
    static void set_framebuffer_info(multiboot_tag_framebuffer *_framebuffer_info);
};
