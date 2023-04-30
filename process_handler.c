typedef struct {
    int id;
    int state; // 0: ready, 1: running, 2: waiting, 3: terminated
    void (*function)(void);
} Process;

#define MAX_PROCESSES 10

Process processes[MAX_PROCESSES];
int current_process = 0;

int add_process(void (*function)(void)) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == 3) {
            processes[i].id = i;
            processes[i].state = 0;
            processes[i].function = function;
            return i;
        }
    }
    return -1; // No available process slot
}

void scheduler() {
    int next_process = (current_process + 1) % MAX_PROCESSES;
    while (processes[next_process].state != 0) {
        next_process = (next_process + 1) % MAX_PROCESSES;
    }
    processes[current_process].state = 0;
    processes[next_process].state = 1;
    current_process = next_process;
}

void run_process() {
    if (processes[current_process].state == 1) {
        processes[current_process].function();
    }
}

void terminate_process(int id) {
    if (id >= 0 && id < MAX_PROCESSES) {
        processes[id].state = 3;
    }
}
