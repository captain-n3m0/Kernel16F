#ifndef STRING_H
#define STRING_H

size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
int memcmp(const void* ptr1, const void* ptr2, size_t num);

#endif
