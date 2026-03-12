#include <stdint.h>
#include <stddef.h>
#include <utils/string.h>

char *int_to_str(int number)
{
    bool isNumberNegative = false;
    if (number == 0)
    {
        return (char *)"0";
    }

    if (number < 0)
    {
        isNumberNegative = true;
        number = -number;
    }

    char reverse_buffer[32];
    size_t index = 0;

    for (size_t i = number; i != 0; i /= 10)
    {
        reverse_buffer[index++] = (char)('0' + (i % 10));
    }
    reverse_buffer[index] = '\0';

    static char int_to_str_buffer[32];
    size_t reverse_index = (isNumberNegative ? 1 : 0);
    size_t rev_index = 0;
    if (isNumberNegative)
    {
        int_to_str_buffer[0] = '-';
    }
    while (reverse_buffer[rev_index] != '\0')
    {
        int_to_str_buffer[reverse_index] = reverse_buffer[index - 1 - rev_index];
        reverse_index++;
        rev_index++;
    }
    int_to_str_buffer[reverse_index] = '\0';
    return int_to_str_buffer;
}

char *hex_to_str(uint64_t hex)
{
    static char hex_to_str_buffer[32];
    size_t index = 2;
    size_t rev_index = 0;
    char reverse_hex_to_str_buffer[32];
    size_t reverse_index = 0;

    hex_to_str_buffer[0] = '0';
    hex_to_str_buffer[1] = 'x';

    for (size_t index = hex; index != 0; index /= 16)
    {
        uint64_t current_number = index % 16;
        if (current_number <= 9)
        {
            reverse_hex_to_str_buffer[reverse_index++] = (char)('0' + current_number);
        }
        else
        {
            reverse_hex_to_str_buffer[reverse_index++] = (char)('A' + (current_number - 10));
        }
    }
    reverse_hex_to_str_buffer[reverse_index] = '\0';

    while (reverse_hex_to_str_buffer[rev_index] != '\0')
    {
        hex_to_str_buffer[index++] = reverse_hex_to_str_buffer[reverse_index - 1 - rev_index];
        rev_index++;
    }
    hex_to_str_buffer[index] = '\0';
    return hex_to_str_buffer;
}

int memcmp(const void *a, const void *b, size_t n)
{
    const uint8_t *pa = (const uint8_t *)a;
    const uint8_t *pb = (const uint8_t *)b;
    for (size_t i = 0; i < n; i++)
    {
        if (pa[i] != pb[i])
            return pa[i] - pb[i];
    }
    return 0;
}