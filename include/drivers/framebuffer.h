#pragma once
#include <limine.h>

enum class Colors : uint32_t
{
    White = 0x00FFFFFF,
    Red = 0x00FF0000,
    Green = 0x0000FF00,
    Blue = 0x000000FF,
    Yellow = 0x00FFFF00,
    Black = 0x00000000,
};

class Framebuffer
{
private:
    static limine_framebuffer_response *fb_response;
    static uint64_t monitor_count;

    //* Cursor
    static uint32_t cursor_x;
    static uint32_t cursor_y;

    static void setMonitorCount();
    static void putPixel(uint32_t x, uint32_t y, Colors color);

public:
    static void init(limine_framebuffer_response *_fb_response);
    static void putChar(const char c, Colors color);
    static void fbPrint(const char *c, Colors color = Colors::White);
};
