#include <stdarg.h>
#include <stdint.h>
#include <drivers/framebuffer.h>
#include <utils/string.h>
#include <utils/io.h>

/**
 * @brief Formatted print to framebuffer
 *
 * Format specifiers:
 *   @param %s string (const char*)
 *   @param %d signed decimal (int)
 *   @param %x hexadecimal (uint64_t), prefixed with 0x
 *   @param %c color (Colors), applies to all following output
 *   @param %% literal % character
 */
void print(const char *input, ...)
{
    int decimal;
    char *string;
    uint64_t hex;
    Colors color = Colors::White;
    va_list ap;
    va_start(ap, input);
    while (*input)
    {
        if (*input == '%')
        {
            input++;
            switch (*input++)
            {
            case 'c':
                color = va_arg(ap, Colors);
                break;
            case 's':
                string = va_arg(ap, char *);
                Framebuffer::fbPrint(string, color);
                break;

            case 'd':
                decimal = va_arg(ap, int);
                Framebuffer::fbPrint(int_to_str(decimal), color);
                break;

            case 'x':
                hex = va_arg(ap, uint64_t);
                Framebuffer::fbPrint(hex_to_str(hex), color);
                break;

            case '%':
                Framebuffer::fbPrint("%", color);
                break;

            default:
                break;
            }
        }
        else
        {
            Framebuffer::putChar(*input, Colors::White);
            input++;
        }
    }

    va_end(ap);
}

void parse_level_prefix(Log *log)
{
    switch (log->type)
    {
    case LogType::INFO:
        log->type_str = "INFO";
        log->color = Colors::White;
        break;
    case LogType::WARN:
        log->type_str = "WARN";
        log->color = Colors::Yellow;
        break;
    case LogType::ERR:
        log->type_str = "ERROR";
        log->color = Colors::Red;
        break;
    }
}

/**
 * @brief Writes a formatted log message to the framebuffer
 *
 * @param type  log level (LogType::INFO, LogType::WARN, LogType::ERR)
 * @param tag   module or component name (e.g. "PMM", "Framebuffer")
 * @param message  message to display
 */
void log(LogType type, const char *tag, const char *message)
{
    // TODO: Add a buffer that write log to a file
    Log log;
    log.type = type;
    log.tag = tag;
    log.message = message;
    parse_level_prefix(&log);
    print("%c[ %s ] %s ==> %s \n", log.color, log.type_str, log.tag, log.message);
}