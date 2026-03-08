#include <stdarg.h>
#include <stdint.h>
#include <drivers/framebuffer.h>
#include <utils/string.h>

void print(const char *input, ...)
{
    int decimal;
    char *string;
    uint64_t hex;
    va_list ap;
    va_start(ap, input);
    while (*input)
    {
        if (*input == '%')
        {
            input++;
            switch (*input++)
            {
            case 's':
                string = va_arg(ap, char *);
                Framebuffer::fbPrint(string, Colors::White);
                break;

            case 'd':
                decimal = va_arg(ap, int);
                Framebuffer::fbPrint(int_to_str(decimal), Colors::White);
                break;

            case 'x':
                hex = va_arg(ap, uint64_t);
                Framebuffer::fbPrint(hex_to_str(hex), Colors::White);
                break;

            case '%':
                Framebuffer::fbPrint("%", Colors::White);
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