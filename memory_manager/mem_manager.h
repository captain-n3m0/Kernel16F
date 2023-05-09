#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stddef.h>

/* Initialize the memory manager */
void init_memory_manager(size_t total_size);

/* Allocate memory from the heap */
void* heap_alloc(size_t size);

/* Free memory from the heap */
void heap_free(void* ptr);

/* Allocate memory from the kheap */
void* kheap_alloc(size_t size);

/* Free memory from the kheap */
void kheap_free(void* ptr);

#endif /* MEM_MANAGER_H */
