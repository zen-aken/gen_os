#include <utils.h>
#include <drivers/framebuffer.h>

uint64_t align(uint64_t variable_size, uint8_t align_size)
{
    return ((variable_size + (align_size - 1)) & ~(align_size - 1));
}

void print(const char *c, Colors color)
{
    Framebuffer::print(c, color);
}

void newLine()
{
    Framebuffer::putChar('\n', Colors::White);
}