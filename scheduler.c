#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_HISTORY_LENGTH 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

char* history[MAX_HISTORY_LENGTH];
int history_index = 0;

char* username = "admin";
char* password = "password";

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
    if (history_index == MAX_HISTORY_LENGTH) {
        free(history[0]);
        for (int i = 0; i < MAX_HISTORY_LENGTH - 1; i++) {
            history[i] = history[i+1];
        }
        history_index--;
    }
    history[history_index++] = strdup(command);
}

void print_history() {
    for (int i = 0; i < history_index; i++) {
        printf("%d: %s\n", i+1, history[i]);
    }
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

    // Authenticate the user
    char input_username[MAX_USERNAME_LENGTH];
    char input_password[MAX_PASSWORD_LENGTH];
    do {
        printf("Username: ");
        scanf("%s", input_username);
        printf("Password: ");
        scanf("%s", input_password);
    } while (strcmp(input_username, username) != 0 || strcmp(input_password, password) != 0);

    // Print the welcome message
    print_string("Welcome to the command line interface!");

    // Initialize the task list
    void (*tasks[])() = {idle_task, task1, task2, task3};
    int num_tasks = sizeof(tasks) / sizeof(tasks[0]);
    int current_task = 0;

    // Loop to accept user commands
    while (1) {
        char command[MAX_COMMAND_LENGTH];
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
        } else if (strcmp(command, "cat") == 0) {
            // Print the contents of a file
            char filename[MAX_COMMAND_LENGTH];
            print_string("\nEnter filename: ");
            scanf("%s", filename);
            FILE* file = fopen(filename, "r");
            if (file == NULL) {
                print_string("\nFile not found.");
            } else {
                char line[MAX_COMMAND_LENGTH];
                while (fgets(line, MAX_COMMAND_LENGTH, file) != NULL) {
                    printf("%s", line);
                }
                fclose(file);
            }
        } else if (strcmp(command, "echo") == 0) {
            // Write a string to a file
            char filename[MAX_COMMAND_LENGTH];
            print_string("\nEnter filename: ");
            scanf("%s", filename);
            char text[MAX_COMMAND_LENGTH];
            print_string("\nEnter text: ");
            scanf("%s", text);
            FILE* file = fopen(filename, "w");
            if (file == NULL) {
                print_string("\nError creating file.");
            } else {
                fprintf(file, "%s", text);
                fclose(file);
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
