#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../headers/stdint.h"

// Initialize the keyboard driver
void keyboard_init();

// Map a scancode to a character based on the keyboard layout
char map_scancode(uint8_t scancode);

// Get a character from the keyboard buffer
char getc();

// Read a line of input from the keyboard
int gets(char* buffer, int length);

#endif
