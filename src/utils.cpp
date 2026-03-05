#include <utils.h>

uint64_t align(uint64_t variable_size, uint8_t align_size)
{
    return ((variable_size + (align_size - 1)) & ~(align_size - 1));
}