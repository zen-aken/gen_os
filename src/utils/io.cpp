#include <stdarg.h>
#include <stdint.h>
#include <drivers/framebuffer.h>
#include <utils/string.h>
#include <utils/io.h>

/**
 * @brief Sends a byte to the specified I/O port
 * @param port Target I/O port (16-bit)
 * @param value Byte to send
 */
void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * @brief Reads a byte from the specified I/O port
 * @param port Source I/O port (16-bit)
 * @return Byte read from port
 */
uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

/**
 * @brief Small delay for legacy hardware synchronization
 * Writes to unused port 0x80 to waste ~1-4 microseconds
 */
void io_wait()
{
    outb(0x80, 0);
}

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
            Framebuffer::putChar(*input, color);
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
void log(LogType type, bool is_log, const char *tag, const char *message, ...)
{
    // TODO: Add a buffer that write log to a file
    Log log;
    log.type = type;
    log.tag = tag;
    log.message = message;
    parse_level_prefix(&log);
    print("%c[ %s ] %s ==> ", log.color, log.type_str, log.tag);
    va_list args;
    va_start(args, message);
    print(message, args);
    va_end(args);

    // for log
    // void (*log_or_print)(const char *, ...);
    if (is_log)
    {
        // write to log manager
    }
}