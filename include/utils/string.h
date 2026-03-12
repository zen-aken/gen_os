#pragma once
#include <stddef.h>

char *int_to_str(int number);
char *hex_to_str(uint64_t hex);
int memcmp(const void *a, const void *b, size_t n);
