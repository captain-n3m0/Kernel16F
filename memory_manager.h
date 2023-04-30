#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

// Define the size of the memory pool
#define MEMORY_POOL_SIZE 1024 * 1024

// Define the memory pool
static uint8_t memory_pool[MEMORY_POOL_SIZE];

// Define a struct for a memory block header
struct memory_block_header {
    size_t size;
    bool is_free;
    struct memory_block_header* next;
};

// Define a global variable for the memory block list
static struct memory_block_header* memory_block_list = NULL;

// Define a function to initialize the memory manager
void init_memory_manager();

// Define a function to allocate memory
void* allocate_memory(size_t size);

// Define a function to free memory
void free_memory(void* ptr);

// Define a function to shutdown the memory manager
void shutdown_memory_manager();

#endif
