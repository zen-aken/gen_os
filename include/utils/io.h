#pragma once
#include <drivers/framebuffer.h>

enum class LogType
{
    INFO,
    WARN,
    ERR
};

struct Log
{
    Colors color;
    LogType type;
    const char *type_str;
    const char *tag;
    const char *message;
};

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

void print(const char *input, ...);
void log(LogType type, bool is_log, const char *tag, const char *message, ...);