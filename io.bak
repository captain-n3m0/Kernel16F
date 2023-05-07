#include <stdint.h>

// Define the I/O ports for reading from and writing to the VGA text buffer
#define VGA_PORT_DATA 0x3D5
#define VGA_PORT_INDEX 0x3D4
#define VGA_INDEX_HIGH_BYTE 0x0E
#define VGA_INDEX_LOW_BYTE 0x0F
static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Define the colors for the VGA text buffer
static const uint8_t VGA_COLOR_BLACK = 0;
static const uint8_t VGA_COLOR_WHITE = 15;

// Define a function to read a character from the keyboard
char getchar() {
    // Wait for a key to be pressed
    while ((inb(0x64) & 0x01) == 0);

    // Read the key from the keyboard
    return inb(0x60);
}

// Define a function to write a character to the screen
void putchar(char character) {
    // Write the character to the VGA text buffer
    static size_t row = 0;
    static size_t column = 0;
    uint16_t color = VGA_COLOR_BLACK << 4 | VGA_COLOR_WHITE;
    if (character == '\n') {
        row++;
        column = 0;
    } else {
        VGA_BUFFER[row * VGA_WIDTH + column] = (uint16_t)character | color << 8;
        column++;
        if (column == VGA_WIDTH) {
            row++;
            column = 0;
        }
    }
    if (row == VGA_HEIGHT) {
        row = 0;
        column = 0;
    }

    // Update the VGA text buffer cursor position
    uint16_t cursor_position = row * VGA_WIDTH + column;
    outb(VGA_PORT_INDEX, VGA_INDEX_HIGH_BYTE);
    outb(VGA_PORT_DATA, cursor_position >> 8);
    outb(VGA_PORT_INDEX, VGA_INDEX_LOW_BYTE);
    outb(VGA_PORT_DATA, cursor_position & 0xFF);
}
