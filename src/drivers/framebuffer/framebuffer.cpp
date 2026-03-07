#include <drivers/framebuffer.h>
//* required for font8x16.h
#define FONT8x16_IMPLEMENTATION
#include <drivers/fonts/font8x16.h>
#include <stddef.h>

limine_framebuffer_response *Framebuffer::fb_response = 0;
uint64_t Framebuffer::monitor_count = 0;

//* Cursor
uint32_t Framebuffer::cursor_x;
uint32_t Framebuffer::cursor_y;

void Framebuffer::setMonitorCount()
{
    monitor_count = fb_response->framebuffer_count;
}

void Framebuffer::init(limine_framebuffer_response *_fb_response)
{
    fb_response = _fb_response;
    setMonitorCount();
    print("Hi from kernel with framebuffer");
}

void Framebuffer::putPixel(uint32_t x, uint32_t y, uint32_t color)
{
    uint32_t *pixel = (uint32_t *)((uint8_t *)fb_response->framebuffers[0]->address + y * fb_response->framebuffers[0]->pitch + x * 4);
    *pixel = color;
}

void Framebuffer::putChar(uint8_t c, uint32_t color)
{
    for (uint32_t row = 0; row < 16; row++)
    {
        for (uint32_t col = 0; col < 8; col++)
        {
            if (font8x16[c][row] & (1 << (7 - col)))
            {
                putPixel(cursor_x + col, cursor_y + row, color);
            }
        }
    }
    cursor_x += 8;
    if (cursor_x >= fb_response->framebuffers[0]->width - 8)
    {
        cursor_x = 0;
        cursor_y += 16;
    }
}

void Framebuffer::print(char *c, uint32_t color)
{
    while (*c)
    {
        putChar(*c, color);
        c++;
    }
}