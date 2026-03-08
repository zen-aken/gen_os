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

void print(const char *input, ...);
void log(LogType type, const char *tag, const char *message);