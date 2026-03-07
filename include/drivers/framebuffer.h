#include <limine.h>

class Framebuffer
{
private:
    static limine_framebuffer_response *fb_response;
    static uint64_t monitor_count;

    //* Cursor
    static uint32_t cursor_x;
    static uint32_t cursor_y;

    static void setMonitorCount();
    static void putPixel(uint32_t x, uint32_t y, uint32_t color);
    static void putChar(uint8_t c, uint32_t color);

public:
    static void init(limine_framebuffer_response *_fb_response);
    static void print(char *c, uint32_t color = 0x00FFFFFF);
};
