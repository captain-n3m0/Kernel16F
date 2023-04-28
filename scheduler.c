#include "memory_manager.h"

#define KERNEL_LOAD_ADDRESS 0x1000
#define STACK_SIZE 1024

// Define a task structure to hold information about each task
typedef struct task {
    uint32_t id;
    uint32_t esp;
    uint32_t cr3;
    uint32_t state;
} task_t;

// Define constants for the task states
#define TASK_STATE_READY 0
#define TASK_STATE_RUNNING 1
#define TASK_STATE_BLOCKED 2

// Define a variable to hold the current task ID
static uint32_t current_task_id = 0;

// Define an array to hold the tasks
static task_t tasks[5];

// Define the kernel stack and stack pointer
static uint8_t kernel_stack[STACK_SIZE];
static uint32_t kernel_stack_pointer = (uint32_t) kernel_stack + STACK_SIZE;

// Define the task stacks and stack pointers
static uint8_t task1_stack[STACK_SIZE];
static uint32_t task1_stack_pointer = (uint32_t) task1_stack + STACK_SIZE;

static uint8_t task2_stack[STACK_SIZE];
static uint32_t task2_stack_pointer = (uint32_t) task2_stack + STACK_SIZE;

static uint8_t task3_stack[STACK_SIZE];
static uint32_t task3_stack_pointer = (uint32_t) task3_stack + STACK_SIZE;

static uint8_t task4_stack[STACK_SIZE];
static uint32_t task4_stack_pointer = (uint32_t) task4_stack + STACK_SIZE;

// Define the function prototypes for the tasks
void task1();
void task2();
void task3();
void task4();

// Define the scheduler function
void scheduler() {
    // Save the current task's ESP and CR3
    tasks[current_task_id].esp = read_esp();
    tasks[current_task_id].cr3 = read_cr3();

    // Find the next ready task
    uint32_t next_task_id = (current_task_id + 1) % 5;
    while (tasks[next_task_id].state != TASK_STATE_READY) {
        next_task_id = (next_task_id + 1) % 5;
    }

    // Switch to the next task
    tasks[current_task_id].state = TASK_STATE_READY;
    tasks[next_task_id].state = TASK_STATE_RUNNING;
    current_task_id = next_task_id;
    write_esp(tasks[current_task_id].esp);
    write_cr3(tasks[current_task_id].cr3);
}

// Define the task functions
void task1() {
    while (1) {
        // Task 1 behavior here
        scheduler();
    }
}

void task2() {
    while (1) {
        // Task 2 behavior here
        scheduler();
    }
}

void task3() {
    while (1) {
        // Task 3 behavior here
        scheduler();
    }
}

void task4() {
    while (1) {
        // Task 4 behavior here
        scheduler();
    }
}

// Define the kernel function
void kernel() {
    // Initialize the data segment register to 0x0000
    asm("mov $0x0000, %ax");
    asm("mov %ax, %ds");

    // Initialize the task structures
    tasks[0].id = 0;
    tasks[0].esp = kernel_stack_pointer;
    tasks[0].cr3 = read_cr3();
    tasks[0].state = TASK_STATE_RUNNING;

    tasks[1].id = 1;
    tasks[1].esp = task1_stack_pointer;
    tasks[1].cr3 = setup
// Define the remaining tasks
    tasks[1].state = TASK_STATE_READY;
    tasks[2].id = 2;
    tasks[2].esp = task2_stack_pointer;
    tasks[2].cr3 = setup_page_directory();
    tasks[2].state = TASK_STATE_READY;
    tasks[3].id = 3;
    tasks[3].esp = task3_stack_pointer;
    tasks[3].cr3 = setup_page_directory();
    tasks[3].state = TASK_STATE_READY;
    tasks[4].id = 4;
    tasks[4].esp = task4_stack_pointer;
    tasks[4].cr3 = setup_page_directory();
    tasks[4].state = TASK_STATE_READY;
// Set up the kernel stack pointer
kernel_stack_pointer -= sizeof(uint32_t);
*(uint32_t*) kernel_stack_pointer = (uint32_t) &scheduler;

// Set up the task 1 stack pointer
task1_stack_pointer -= sizeof(uint32_t);
*(uint32_t*) task1_stack_pointer = (uint32_t) &task1;

// Set up the task 2 stack pointer
task2_stack_pointer -= sizeof(uint32_t);
*(uint32_t*) task2_stack_pointer = (uint32_t) &task2;

// Set up the task 3 stack pointer
task3_stack_pointer -= sizeof(uint32_t);
*(uint32_t*) task3_stack_pointer = (uint32_t) &task3;

// Set up the task 4 stack pointer
task4_stack_pointer -= sizeof(uint32_t);
*(uint32_t*) task4_stack_pointer = (uint32_t) &task4;

// Set up the interrupt descriptor table
setup_idt();

// Start the scheduler
scheduler();
}

// Define the main function
int main() {
// Load the kernel
load_kernel(KERNEL_LOAD_ADDRESS);
// Jump to the kernel
jump_to_kernel(KERNEL_LOAD_ADDRESS);

// We should never get here
return 0;
}