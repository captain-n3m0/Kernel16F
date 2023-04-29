#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024
#define KERNEL_VIRTUAL_BASE 0xC0000000
#define KERNEL_PHYSICAL_BASE 0x00100000

// Page directory entry structure
typedef struct {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t write_through : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t reserved : 1;
    uint32_t page_size : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t page_table_base : 20;
} page_directory_entry_t;

// Page table entry structure
typedef struct {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t write_through : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t reserved : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t physical_address : 20;
} page_table_entry_t;

// Page directory
page_directory_entry_t page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

// Page tables
page_table_entry_t page_tables[PAGE_TABLE_ENTRIES][PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

// Initialize the memory manager
void init_memory_manager() {
    // Map the kernel pages
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        page_tables[0][i].present = 1;
        page_tables[0][i].read_write = 1;
        page_tables[0][i].physical_address = i;
    }
    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
        page_directory[i].present = 0;
        page_directory[i].page_table_base = (uint32_t) &page_tables[i];
    }
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].page_size = 1;
    page_directory[0].page_table_base = (uint32_t) &page_tables[0];
    asm("mov %0, %%cr3" : : "r" (page_directory));
    asm("mov %cr0, %eax");
    asm("or $0x80000000, %eax");
    asm("mov %eax, %cr0");
}

// Allocate a page of memory
void* allocate_page() {
    // Find a free page table entry
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        for (int j = 0; j < PAGE_TABLE_ENTRIES; j++) {
            if (!page_tables[i][j].present) {
                // Mark the page as present and return its virtual address
                page_tables[i][j].present = 1;
                page_tables[i][j].read_write = 1;
                page_tables[i][j].physical_address = i * PAGE_TABLE_ENTRIES + j;
                return (void*) (KERNEL_VIRTUAL_BASE + i * PAGE_SIZE * PAGE_TABLE_ENTRIES + j * PAGE_SIZE);
            }
        }
    }
    // No free page table entry found
    return NULL;
}

// Free a page of memory
void free_page(void* page) {
    // Calculate the page table index and entry index
    uint32_t page_index = (uint32_t) page - KERNEL_VIRTUAL_BASE;
    uint32_t page_table_index = page_index / (PAGE_SIZE * PAGE_TABLE_ENTRIES);
    uint32_t page_entry_index = (page_index / PAGE_SIZE) % PAGE_TABLE_ENTRIES;
    // Mark the page as not present
    page_tables[page_table_index][page_entry_index].present = 0;
}
