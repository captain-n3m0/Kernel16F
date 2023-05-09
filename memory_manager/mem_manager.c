#include "mem_manager.h"
#include <stdlib.h>
#include <string.h>

/* Define the heap and kheap */
static void* heap;
static void* kheap;

/* Define the heap size and kheap size */
static size_t heap_size;
static size_t kheap_size;

/* Initialize the memory manager */
void init_memory_manager(size_t total_size) {
    /* Allocate memory for the heap and kheap */
    heap_size = total_size / 2;
    kheap_size = total_size - heap_size;
    heap = malloc(heap_size);
    kheap = malloc(kheap_size);

    /* Initialize the heap and kheap */
    memset(heap, 0, heap_size);
    memset(kheap, 0, kheap_size);
}

/* Allocate memory from the heap */
void* heap_alloc(size_t size) {
    return malloc(size);
}

/* Free memory from the heap */
void heap_free(void* ptr) {
    free(ptr);
}

/* Allocate memory from the kheap */
void* kheap_alloc(size_t size) {
    void* ptr = NULL;
    /* Find the first free block of memory */
    for (size_t i = 0; i < kheap_size; i += sizeof(size_t)) {
        size_t* block_size = (size_t*)((char*)kheap + i);
        if (*block_size == 0) {
            /* If the block is free, mark it as used and return its address */
            *block_size = size;
            ptr = (char*)kheap + i + sizeof(size_t);
            break;
        }
    }
    return ptr;
}

/* Free memory from the kheap */
void kheap_free(void* ptr) {
    /* Mark the block as free */
    size_t* block_size = (size_t*)((char*)ptr - sizeof(size_t));
    *block_size = 0;
}
