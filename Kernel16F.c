#include "Kernel16F.h"
#include "display/display.h"
#include "filesystem/filesystem.h"
#include "disk_operations/disk.h"
#include "memory_manager/mem_manager.h"
#include "string/string.h"
#include "process_manager/proc_man.h"
#include "cli_interface/cli.h"
#include "drivers/keyboard_driver.h"
void main()
{
    // Create the kernels heap
    init_memory_manager(4 * 1024 * 1024);

    // Load all the integrated filesystems
    fs_load();

    // Search for disks and initialize them
    disk_search_and_init();

    // Initialise the process manager
    process_manager_init();

    // Initialise the display
    display_init();

    // Initialise the CLI
    cli_init();

    // Initialise the keyboard
    keyboard_init();

    {
        int f = fopen("0:/START", 'r');
        if (f < 0)
        {
            panic("Failed to find \"START\" program\r\n");
        }
    }


    // Infinite loop so we never loop off into the universe
    while(1) {

    }

}

void panic(char *message)
{
    printf("!!!KERNEL PANIC!!!: ");
    printf(message);
    while (1)
    {
    }
}
