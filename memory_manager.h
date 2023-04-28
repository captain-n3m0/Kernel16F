#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

/* Size of each memory block */
#define BLOCK_SIZE 4096

/* Number of memory blocks available for allocation */
#define NUM_BLOCKS 1024

/* Start address of the memory pool */
#define MEMORY_START_ADDRESS 0x100000

/* Structure representing a memory block */
typedef struct block {
    uint8_t data[BLOCK_SIZE];
    struct block* next;
} block_t;

/* Initialize the memory pool */
void mem_init();

/* Allocate a memory block */
void* mem_alloc();

/* Free a previously allocated memory block */
void mem_free(void* ptr);

#endif /* MEMORY_MANAGER_H */
