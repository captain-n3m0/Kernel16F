#ifndef _STDDEF_H
#define _STDDEF_H

typedef unsigned int size_t;
typedef int ptrdiff_t;
typedef unsigned int wchar_t;

#define NULL ((void *)0)
#define offsetof(type, member) ((size_t) &((type *)0)->member)

#endif /* _STDDEF_H */
