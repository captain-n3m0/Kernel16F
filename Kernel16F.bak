#include "memory_manager.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "io.h"

// Define the VGA text buffer
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Define the colors for the VGA text buffer
static const uint8_t VGA_COLOR_BLACK = 0;
static const uint8_t VGA_COLOR_WHITE = 15;

// Define a struct for the VGA text buffer cursor
struct vga_cursor {
    size_t row;
    size_t column;
};

// Define a global variable for the VGA text buffer cursor
static struct vga_cursor vga_cursor = {0, 0};

// Define a function to clear the VGA text buffer
static void clear_vga_buffer() {
    for (size_t row = 0; row < VGA_HEIGHT; row++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            const size_t index = row * VGA_WIDTH + column;
            VGA_BUFFER[index] = (uint16_t)' ' | (uint16_t)VGA_COLOR_BLACK << 8;
        }
    }
}

// Define a function to write a character to the VGA text buffer
static void write_vga_character(char character, uint8_t color) {
    if (character == '\n') {
        // Move the cursor to the beginning of the next row
        vga_cursor.row++;
        vga_cursor.column = 0;
    } else {
        // Write the character to the current position in the VGA text buffer
        const size_t index = vga_cursor.row * VGA_WIDTH + vga_cursor.column;
        VGA_BUFFER[index] = (uint16_t)character | (uint16_t)color << 8;

        // Move the cursor to the next position in the VGA text buffer
        vga_cursor.column++;
        if (vga_cursor.column == VGA_WIDTH) {
            // Move the cursor to the beginning of the next row
            vga_cursor.row++;
            vga_cursor.column = 0;
        }
    }

    // Check if the cursor is at the bottom of the VGA text buffer
    if (vga_cursor.row == VGA_HEIGHT) {
        // Scroll the VGA text buffer up by one row
        for (size_t row = 1; row < VGA_HEIGHT; row++) {
            for (size_t column = 0; column < VGA_WIDTH; column++) {
                const size_t index = (row - 1) * VGA_WIDTH + column;
                const size_t next_index = row * VGA_WIDTH + column;
                VGA_BUFFER[index] = VGA_BUFFER[next_index];
            }
        }

        // Clear the last row of the VGA text buffer
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + column;
            VGA_BUFFER[index] = (uint16_t)' ' | (uint16_t)VGA_COLOR_BLACK << 8;
        }

        // Move the cursor to the beginning of the last row
        vga_cursor.row = VGA_HEIGHT - 1;
        vga_cursor.column = 0;
    }
}

// Define a function to write a string to the VGA text buffer
static void write_vga_string(const char* string, uint8_t color) {
    for (size_t i = 0; i < strlen(string); i++) {
        write_vga_character(string[i], color);
    }
}

// Define a function to read a line of input from the user
static void read_input_line(char* buffer, size_t buffer_size) {
    size_t i = 0;
    while (i < buffer_size - 1) {
        // Read a character from the user
        char character = getchar();

        // Check if the character is a newline or carriage return
        if (character == '\n' || character == '\r') {
            // Terminate the input line with a null character
            buffer[i] = '\0';
            break;
        }

        // Write the character to the input buffer
        buffer[i] = character;
        i++;

        // Echo the character back to the user
        putchar(character);
    }
}

// Define a function to execute a command
static void execute_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        // Display the help message
        write_vga_string("Available commands:\n", VGA_COLOR_WHITE);
        write_vga_string("  help - display this help message\n", VGA_COLOR_WHITE);
        write_vga_string("  echo - repeat the input text\n", VGA_COLOR_WHITE);
        write_vga_string("  clear - clear the screen\n", VGA_COLOR_WHITE);
    } else if (strcmp(command, "echo") == 0) {
        // Read the input text from the user
        write_vga_string("Enter text to echo: ", VGA_COLOR_WHITE);
        char input[256];
        read_input_line(input, sizeof(input));

        // Echo the input text back to the user
        write_vga_string("You entered: ", VGA_COLOR_WHITE);
        write_vga_string(input, VGA_COLOR_WHITE);
        write_vga_string("\n", VGA_COLOR_WHITE);
    } else if (strcmp(command, "clear") == 0) {
        // Clear the screen
        clear_vga_buffer();
        vga_cursor.row = 0;
        vga_cursor.column = 0;
    } else {
        // Display an error message
        write_vga_string("Unknown command: ", VGA_COLOR_WHITE);
        write_vga_string(command, VGA_COLOR_WHITE);
        write_vga_string("\n", VGA_COLOR_WHITE);
    }
}

// Define the kernel entry point
void kernel_main() {
    // Initialize the memory manager
    init_memory_manager();

    // Clear the VGA text buffer
    clear_vga_buffer();

    // Display the welcome message
    write_vga_string("Welcome to Kernel16F!\n", VGA_COLOR_WHITE);
    write_vga_string("Type 'help' for a list of available commands.\n", VGA_COLOR_WHITE);

    // Read and execute commands from the user
    while (true) {
        // Display the command prompt
        write_vga_string("> ", VGA_COLOR_WHITE);

        // Read the input command from the user
        char input[256];
        read_input_line(input, sizeof(input));

        // Execute the input command
        execute_command(input);
    }
}
