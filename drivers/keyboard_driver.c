#include <stdint.h>
#include "io.h"
#include "interrupts.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_IRQ 1
#define KEYBOARD_BUFFER_SIZE 128

char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int keyboard_buffer_head = 0;
int keyboard_buffer_tail = 0;

void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    // Handle the scancode here
    if (scancode & 0x80) {
        // Key release
    } else {
        // Key press
        char c = map_scancode(scancode);
        if (c != '\0') {
            // Add the character to the keyboard buffer
            keyboard_buffer[keyboard_buffer_head] = c;
            keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        }
    }
}

void keyboard_init() {
    // Enable the keyboard IRQ
    enable_irq(KEYBOARD_IRQ);
}

char map_scancode(uint8_t scancode) {
    // Map the scancode to a character based on the keyboard layout
    // You can add support for different layouts here
    switch (scancode) {
        case 0x01: return '\x1b'; // Escape
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0a: return '9';
        case 0x0b: return '0';
        case 0x0c: return '-';
        case 0x0d: return '=';
        case 0x0e: return '\b'; // Backspace
        case 0x0f: return '\t'; // Tab
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1a: return '[';
        case 0x1b: return ']';
        case 0x1c: return '\n'; // Enter
        case 0x1d: return '\0'; // Left control
        case 0x1e: return 'a';
        case 0x1f: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x27: return ';';
        case 0x28: return '\'';
        case 0x29: return '`';
        case 0x2a: return '\0'; // Left shift
        case 0x2b: return '\\';
        case 0x2c: return 'z';
        case 0x2d: return 'x';
        case 0x2e: return 'c';
        case 0x2f: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x35: return '/';
        case 0x36: return '\0'; // Right shift
        case 0x37: return '*';
        case 0x38: return '\0'; // Left alt
        case 0x39: return ' ';
        case 0x3a: return '\0'; // Caps lock
        case 0x3b: return '\0'; // F1
        case 0x3c: return '\0'; // F2
        case 0x3d: return '\0'; // F3
        case 0x3e: return '\0'; // F4
        case 0x3f: return '\0'; // F5
        case 0x40: return '\0'; // F6
        case 0x41: return '\0'; // F7
        case 0x42: return '\0'; // F8
        case 0x43: return '\0'; // F9
        case 0x44: return '\0'; // F10
        case 0x45: return '\0'; // Num lock
        case 0x46: return '\0'; // Scroll lock
        case 0x47: return '7'; // Home
        case 0x48: return '8'; // Up arrow
        case 0x49: return '9'; // Page up
        case 0x4a: return '-'; // Minus
        case 0x4b: return '4'; // Left arrow
        case 0x4c: return '5'; // Center
        case 0x4d: return '6'; // Right arrow
        case 0x4e: return '+'; // Plus
        case 0x4f: return '1'; // End
        case 0x50: return '2'; // Down arrow
        case 0x51: return '3'; // Page down
        case 0x52: return '0'; // Insert
        case 0x53: return '.'; // Delete
        default: return '\0'; // Unknown scancode
    }
}

char getc() {
    // Get a character from the keyboard buffer
    if (keyboard_buffer_head == keyboard_buffer_tail) {
        return '\0'; // Buffer is empty
    }
    char c = keyboard_buffer[keyboard_buffer_tail];
    keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

int gets(char* buffer, int length) {
    // Read a line of input from the keyboard
    int i = 0;
    while (i < length - 1) {
        char c = getc();
        if (c == '\n') {
            break;
        } else if (c != '\0') {
            buffer[i++] = c;
        }
    }
    buffer[i] = '\0';
    return i;
}
