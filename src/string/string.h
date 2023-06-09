#ifndef STRING_H
#define STRING_H

#include "../headers/stddef.h"

/**
 * Copies up to `n` characters from `src` to `dest`, padding with null characters if necessary.
 * Returns a pointer to `dest`.
 */
char *strncpy(char *dest, const char *src, size_t n);

/**
 * Returns the length of the string `str`, up to a maximum of `max` characters.
 */
size_t strnlen(const char *str, size_t max);

/**
 * Returns true if the given character `c` is a digit (0-9).
 */
int isdigit(char c);

/**
 * Converts the given character `c` to its corresponding numeric value (0-9).
 * Returns -1 if `c` is not a digit.
 */
int tonumericdigit(char c);

void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
#endif
