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
void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while (n-- && (*p1 == *p2)) {
        p1++;
        p2++;
    }
    return (n < 0) ? 0 : (*p1 - *p2);
}
