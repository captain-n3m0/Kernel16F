#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "display/display.h"

void echo(char* args[]) {
    char output[100] = "";
    for (int i = 0; args[i] != NULL; i++) {
        strcat(output, args[i]);
        strcat(output, " ");
    }
    strcat(output, "\n");
    display_write(output);
}

void help(char* args[]) {
    char output[100] = "";
    strcat(output, "Available commands:\n");
    strcat(output, "echo [args...]\n");
    strcat(output, "help\n");
    strcat(output, "version\n");
    display_write(output);
}

void version(char* args[]) {
    char output[100] = "";
    strcat(output, "Kernel16F version 1.0\n");
    display_write(output);
}

void cli_init() {
    display_init();
}

void cli_parse_input(char* input_str) {
    char* args[10];
    char* token = strtok(input_str, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (strcmp(args[0], "echo") == 0) {
        echo(&args[1]);
    } else if (strcmp(args[0], "help") == 0) {
        help(&args[1]);
    } else if (strcmp(args[0], "version") == 0) {
        version(&args[1]);
    } else {
        char output[100] = "";
        strcat(output, "Invalid command. Type 'help' for a list of available commands.\n");
        display_write(output);
    }
}
