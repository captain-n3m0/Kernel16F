#include <stdio.h>
#include <string.h>

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
    asm("iret");
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

void start() {
    // Initialize the data segment register to 0x0000
    asm("mov $0x0000, %ax");
    asm("mov %ax, %ds");

    // Print the welcome message
    print_string("Welcome to the command line interface!");

    // Loop to accept user commands
    while (1) {
        char command[256];
        print_string("\nEnter a command: ");
        scanf("%s", command);

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
            print_string("\nCreating file...");
            // TODO: Implement file creation
        } else if (strcmp(command, "rm") == 0) {
            // Delete a file
            print_string("\nDeleting file...");
            // TODO: Implement file deletion
        } else if (strcmp(command, "rmdir") == 0) {
            // Delete a directory
            print_string("\nDeleting directory...");
            // TODO: Implement directory deletion
        } else if (strcmp(command, "exit") == 0) {
            // Exit the command line interface
            print_string("\nExiting...");
            halt();
        }  else if (strcmp(command, "echo") == 0) {
            // Echo the user input back to the screen
            print_string("\nEnter text to echo: ");
            char text[256];
            scanf("%s", text);
            print_string("\n");
            print_string(text);
        } else {
            // Unknown command
            print_string("\nUnknown command: ");
            print_string(command);
        }
    }
}