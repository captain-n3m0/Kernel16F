#include "string.h"

char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}

size_t strnlen(const char *str, size_t max)
{
    size_t len = 0;

    while (len < max && str[len] != '\0') {
        len++;
    }

    return len;
}

int isdigit(char c)
{
    return c >= '0' && c <= '9';
}

int tonumericdigit(char c)
{
    if (!isdigit(c)) {
        return -1;
    }

    return c - '0';
}
