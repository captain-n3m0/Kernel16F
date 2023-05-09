#include "Kernel16F.h"
#include "display.h"
#include "filesystem/filesystem.h"
#include "disk_operations/disk.h"
#include "memory_manager/mem_manager.h"
#include "string_manipulation/string_utils.h"
#include "process_manager/proc_man.h"
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
    print("!!!KERNEL PANIC!!!: ");
    print(message);
    while (1)
    {
    }
}
