#ifndef PROC_MAN_H
#define PROC_MAN_H

#include "../headers/stdbool.h"
#include "../headers/stddef.h"
#include "../headers/stdint.h"

#define MAX_PROCESSES 1024

typedef struct {
    uint32_t pid;
    bool is_running;
    void (*entry_point)(void);
} process_t;

void process_manager_init(void);
process_t *process_create(void (*entry_point)(void));
void process_start(process_t *process);
void process_stop(process_t *process);
void process_destroy(process_t *process);
process_t *process_get(uint32_t pid);

#endif
