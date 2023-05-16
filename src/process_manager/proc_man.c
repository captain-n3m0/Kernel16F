#include "proc_man.h"
#include "../headers/stdlib.h"
#include "../string/string.h"

static process_t processes[MAX_PROCESSES];
static uint32_t next_pid = 1;

void process_manager_init(void)
{
    memset(processes, 0, sizeof(processes));
}

process_t *process_create(void (*entry_point)(void))
{
    process_t *process = NULL;

    // Find an empty slot in the process table
    for (size_t i = 0; i < MAX_PROCESSES; i++) {
        if (!processes[i].is_running) {
            process = &processes[i];
            break;
        }
    }

    if (!process) {
        return NULL;
    }

    // Initialize the process
    process->pid = next_pid++;
    process->is_running = false;
    process->entry_point = entry_point;

    return process;
}

void process_start(process_t *process)
{
    if (!process) {
        return;
    }

    // Start the process by calling its entry point
    process->is_running = true;
    process->entry_point();
}

void process_stop(process_t *process)
{
    if (!process) {
        return;
    }

    // Stop the process by setting its is_running flag to false
    process->is_running = false;
}

void process_destroy(process_t *process)
{
    if (!process) {
        return;
    }

    // Clear the process's memory and mark it as not running
    memset(process, 0, sizeof(process_t));
}

process_t *process_get(uint32_t pid)
{
    // Find the process with the given PID
    for (size_t i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].pid == pid) {
            return &processes[i];
        }
    }

    return NULL;
}
