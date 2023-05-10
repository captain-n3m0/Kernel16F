#include "../filesystem/filesystem.h"
#include "../types.h"

#define KERNEL16F_FAT16_SIGNATURE 0x29

struct filesystem* fat16_init()
{
    struct filesystem* fat16_fs = kmalloc(sizeof(struct filesystem));
    fat16_fs->open = fat16_open;
    fat16_fs->resolve = fat16_resolve;
    strncpy(fat16_fs->name, "FAT16", sizeof(fat16_fs->name));
    return fat16_fs;
}
