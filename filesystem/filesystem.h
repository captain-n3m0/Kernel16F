#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../disk_operations/disk.h"
#include "../headers/stdio.h""

typedef void* (*FS_OPEN_FUNCTION)(struct disk* disk, char* filename, char mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk);

struct filesystem
{
    FS_RESOLVE_FUNCTION resolve;
    FS_OPEN_FUNCTION open;
    char name[20];
};

void fs_load();
int fopen(char* filename, char mode);
void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);

#endif
