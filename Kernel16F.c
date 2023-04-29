#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory_manager.h"

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define NUM_INODES 64
#define MAX_FILENAME_LENGTH 16
#define MAX_FILE_SIZE (NUM_BLOCKS / 2 * BLOCK_SIZE)

typedef struct {
    int block_number;
    int offset;
} file_pointer;

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    int size;
    int blocks[NUM_BLOCKS / 2];
} inode;

typedef struct {
    char data[BLOCK_SIZE];
} block;

inode inodes[NUM_INODES];
block blocks[NUM_BLOCKS];
int free_blocks[NUM_BLOCKS];
int free_blocks_index = 0;

void kernel_main() {
    // Initialize the memory manager
    init_memory_manager();

    // Allocate some memory
    void* ptr = allocate_memory(1024);

    // Print the address of the allocated memory
    printf("Allocated memory at address %p\n", ptr);

    // Free the allocated memory
    free_memory(ptr);

    // Allocate some more memory
    void* ptr2 = allocate_memory(2048);

    // Print the address of the second allocated memory
    printf("Allocated memory at address %p\n", ptr2);

    // Free the second allocated memory
    free_memory(ptr2);

    // Shutdown the memory manager
    shutdown_memory_manager();
}

void initialize_filesystem() {
    // Initialize the free block list
    for (int i = 0; i < NUM_BLOCKS; i++) {
        free_blocks[i] = i;
    }

    // Initialize the root directory
    strcpy(inodes[0].name, "/");
    inodes[0].size = 0;
    for (int i = 0; i < NUM_BLOCKS / 2; i++) {
        inodes[0].blocks[i] = -1;
    }
}

int allocate_block() {
    if (free_blocks_index == NUM_BLOCKS) {
        return -1; // No free blocks available
    }
    int block_number = free_blocks[free_blocks_index++];
    return block_number;
}

void free_block(int block_number) {
    free_blocks[--free_blocks_index] = block_number;
}

int find_inode(char* path) {
    if (strcmp(path, "/") == 0) {
        return 0; // Root directory
    }
    for (int i = 1; i < NUM_INODES; i++) {
        if (strcmp(path, inodes[i].name) == 0) {
            return i; // Found the inode
        }
    }
    return -1; // Inode not found
}

file_pointer open_file(char* path) {
    int inode_number = find_inode(path);
    if (inode_number == -1) {
        return (file_pointer) {-1, -1}; // File not found
    }
    int block_number = allocate_block();
    if (block_number == -1) {
        return (file_pointer) {-1, -1}; // No free blocks available
    }
    inodes[inode_number].blocks[block_number / 2] = block_number;
    return (file_pointer) {block_number, 0};
}

void close_file(file_pointer fp) {
    free_block(fp.block_number);
}

int read_file(file_pointer fp, char* buffer, int length) {
    if (fp.block_number == -1) {
        return -1; // File not found
    }
    int bytes_read = 0;
    while (bytes_read < length) {
        if (fp.offset == BLOCK_SIZE) {
            // Move to the next block
            int block_number = inodes[fp.block_number].blocks[fp.offset / BLOCK_SIZE];
            if (block_number == -1) {
                break; // End of file
            }
            fp.block_number = block_number;
            fp.offset = 0;
        }
        buffer[bytes_read++] = blocks[fp.block_number].data[fp.offset++];
    }
    return bytes_read;
}

int write_file(file_pointer fp, char* buffer, int length) {
    if (fp.block_number == -1) {
        return -1; // File not found
    }
    int bytes_written = 0;
    while (bytes_written < length) {
        if (fp.offset == BLOCK_SIZE) {
            // Move to the next block
            int block_number = inodes[fp.block_number].blocks[fp.offset / BLOCK_SIZE];
            if (block_number == -1) {
                // Allocate a new block
                block_number = allocate_block();
                if (block_number == -1) {
                    return bytes_written; // No free blocks available
                }
                inodes[fp.block_number].blocks[fp.offset / BLOCK_SIZE] = block_number;
            }
            fp.block_number = block_number;
            fp.offset = 0;
        }
        blocks[fp.block_number].data[fp.offset++] = buffer[bytes_written++];
    }
    if (inodes[fp.block_number].size < fp.offset) {
        inodes[fp.block_number].size = fp.offset;
    }
    return bytes_written;
}

int create_file(char* path) {
    int inode_number = find_inode(path);
    if (inode_number != -1) {
        return -1; // File already exists
    }
    for (int i = 1; i < NUM_INODES; i++) {
        if (inodes[i].name[0] == '\0') {
            strcpy(inodes[i].name, path);
            inodes[i].size = 0;
            for (int j = 0; j < NUM_BLOCKS / 2; j++) {
                inodes[i].blocks[j] = -1;
            }
            return 0; // File created successfully
        }
    }
    return -1; // No free inodes available
}

int delete_file(char* path) {
    int inode_number = find_inode(path);
    if (inode_number == -1) {
        return -1; // File not found
    }
    for (int i = 0; i < NUM_BLOCKS / 2; i++) {
        int block_number = inodes[inode_number].blocks[i];
        if (block_number != -1) {
            free_block(block_number);
        }
    }
    inodes[inode_number].name[0] = '\0';
    inodes[inode_number].size = 0;
    for (int i = 0; i < NUM_BLOCKS / 2; i++) {
        inodes[inode_number].blocks[i] = -1;
    }
    return 0; // File deleted successfully
}

void print_string(char* str) {
    char* video_memory = (char*) 0xB8000;
    while (*str != '\0') {
        *video_memory++ = *str++;
        *video_memory++ = 0x0F; // Set the text color to white on black
    }
}

void halt() {
    asm("cli");
    asm("hlt");
}

void int_handler() {
    asm("push %ax");
    asm("push %bx");
    asm("push %cx");
    asm("push %dx");

    // Check which interrupt occurred
    asm("mov $0x0E, %ah");
    asm("mov $0x00, %bh");
    asm("mov $0x07, %bl");
    asm("mov $'I', %al");
    asm("int $0x10");

    asm("pop %dx");
    asm("pop %cx");
    asm("pop %bx");
    asm("pop %ax");
}

void sys_call() {
    asm("push %ax");
    asm("push %bx");
    asm("push %cx");
    asm("push %dx");

    // Check which system call was requested
    asm("mov $0x0E, %ah");
    asm("mov $0x00, %bh");
    asm("mov $0x07, %bl");
    asm("mov $'S', %al");
    asm("int $0x10");

    asm("pop %dx");
    asm("pop %cx");
    asm("pop %bx");
    asm("pop %ax");
}

void add_to_history(char* command) {
    // TODO: Implement command history
}

void print_history() {
    // TODO: Implement command history
}

void idle_task() {
    while (1) {
        // Do nothing
    }
}

void task1() {
    while (1) {
        print_string("\nTask 1 running...");
    }
}

void task2() {
    while (1) {
        print_string("\nTask 2 running...");
    }
}

void task3() {
    while (1) {
        print_string("\nTask 3 running...");
    }
}

void start() {
    // Initialize the data segment register to 0x0000
    asm("mov $0x0000, %ax");
    asm("mov %ax, %ds");

    // Initialize the filesystem
    initialize_filesystem();

    // Print the welcome message
    print_string("Welcome to Kernel16F!");

    // Initialize the task list
    void (*tasks[])() = {idle_task, task1, task2, task3};
    int num_tasks = sizeof(tasks) / sizeof(tasks[0]);
    int current_task = 0;

    // Loop to accept user commands
    while (1) {
        char command[MAX_FILENAME_LENGTH];
        print_string("\nEnter a command: ");
        scanf("%s", command);

        // Add the command to the history
        add_to_history(command);

        if (strcmp(command, "ls") == 0) {
            // List the files in the current directory
            print_string("\nListing files...");
            // TODO: Implement file listing
        } else if (strcmp(command, "cd") == 0) {
            // Change the current directory
            print_string("\nChanging directory...");
            // TODO: Implement directory changing
        } else if (strcmp(command, "mkdir") == 0) {
            // Create a new directory
            print_string("\nCreating directory...");
            // TODO: Implement directory creation
        } else if (strcmp(command, "touch") == 0) {
            // Create a new file
            char filename[MAX_FILENAME_LENGTH];
            print_string("\nEnter filename: ");
            scanf("%s", filename);
            if (create_file(filename) == 0) {
                print_string("\nFile created successfully.");
            } else {
                print_string("\nError creating file.");
            }
        } else if (strcmp(command, "rm") == 0) {
            // Delete a file
            char filename[MAX_FILENAME_LENGTH];
            print_string("\nEnter filename: ");
            scanf("%s", filename);
            if (delete_file(filename) == 0) {
                print_string("\nFile deleted successfully.");
            } else {
                print_string("\nError deleting file.");
            }
        } else if (strcmp(command, "cat") == 0) {
            // Print the contents of a file
            char filename[MAX_FILENAME_LENGTH];
            print_string("\nEnter filename: ");
            scanf("%s", filename);
            file_pointer fp = open_file(filename);
            if (fp.block_number == -1) {
                print_string("\nFile not found.");
            } else {
                char buffer[MAX_FILE_SIZE];
                int bytes_read = read_file(fp, buffer, MAX_FILE_SIZE);
                buffer[bytes_read] = '\0';
                printf("%s", buffer);
                close_file(fp);
            }
        } else if (strcmp(command, "echo") == 0) {
            // Write a string to a file
            char filename[MAX_FILENAME_LENGTH];
            print_string("\nEnter filename: ");
            scanf("%s", filename);
            char text[MAX_FILE_SIZE];
            print_string("\nEnter text: ");
            scanf("%s", text);
            file_pointer fp = open_file(filename);
            if (fp.block_number == -1) {
                print_string("\nError opening file.");
            } else {
                int bytes_written = write_file(fp, text, strlen(text));
                if (bytes_written == strlen(text)) {
                    print_string("\nText written to file.");
                } else {
                    print_string("\nError writing to file.");
                }
                close_file(fp);
            }
        } else if (strcmp(command, "history") == 0) {
            // Print the command history
            print_history();
        } else if (strcmp(command, "exit") == 0) {
            // Exit the command line interface
            print_string("\nExiting...");
            halt();
        } else {
            // Unknown command
            print_string("\nUnknown command: ");
            print_string(command);
        }

        // Switch to the next task
        current_task = (current_task + 1) % num_tasks;
        tasks[current_task]();
    }
}
