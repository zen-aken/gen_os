#include <boot/multiboot2.h>
#include <drivers/framebuffer.h>

multiboot_tag_framebuffer *Framebuffer::framebuffer_info = nullptr;

void Framebuffer::set_framebuffer_info(multiboot_tag_framebuffer *_framebuffer_info)
{
    framebuffer_info = _framebuffer_info;
}