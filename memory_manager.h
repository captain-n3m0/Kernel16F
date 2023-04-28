#define MEMORY_SIZE 1024 // Size of the memory pool in bytes

// Memory pool
char memory[MEMORY_SIZE];

// Allocation table
int allocation_table[MEMORY_SIZE];

// Initializes the memory manager
void init_memory_manager() {
    memset(memory, 0, MEMORY_SIZE);
    memset(allocation_table, 0, MEMORY_SIZE * sizeof(int));
}

// Allocates a block of memory of the specified size
void* allocate_memory(int size) {
    // Find the first available block of memory
    int block_start = -1;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (allocation_table[i] == 0) {
            // Found an available block
            block_start = i;
            break;
        }
    }

    if (block_start == -1) {
        // No available block found
        return NULL;
    }

    // Check if there is enough space in the memory pool
    if (block_start + size > MEMORY_SIZE) {
        // Not enough space
        return NULL;
    }

    // Allocate the block
    for (int i = block_start; i < block_start + size; i++) {
        allocation_table[i] = 1;
    }

    // Return the pointer to the allocated block
    return &memory[block_start];
}

// Frees a block of memory allocated with allocate_memory()
void free_memory(void* ptr, int size) {
    // Calculate the start index of the block
    int block_start = (int)ptr - (int)memory;

    // Free the block
    for (int i = block_start; i < block_start + size; i++) {
        allocation_table[i] = 0;
    }
}
