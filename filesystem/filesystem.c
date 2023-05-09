#include "file.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "fat16/fat16.h"
#include "disk_operations/disk.h"
#include "string_manipulation/string.h"
#include "status.h"

#define MAX_FILESYSTEMS 10
struct filesystem *filesystems[MAX_FILESYSTEMS];

static struct filesystem **fs_get_free_filesystem()
{
    for (int i = 0; i < MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == NULL)
        {
            return &filesystems[i];
        }
    }

    return NULL;
}

void fs_insert_filesystem(struct filesystem *filesystem)
{
    if (filesystem == NULL)
    {
        panic("NULL filesystem provided");
    }

    struct filesystem **fs = fs_get_free_filesystem();
    if (fs == NULL)
    {
        panic("No more filesystem slots available, failed to register filesystem");
    }

    *fs = filesystem;
    print("Filesystem ");
    print(filesystem->name);
    print(" initialized.\n");
}

/**
 * Loads statically compiled filesystems
 */
static void fs_static_load()
{
    fs_insert_filesystem(fat16_init());
}

void fs_load()
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

/**
 *
 * Tests the given filename to see if the path is a valid format
 * \warning This function does not test if the path exists or not
 * Valid paths
 * 0:/
 * 0:/testing/abc
 * 1:/abc/testing
 *
 * Invalid paths
 * A:/abc
 * B:/
 */
static int fs_valid_path_format(char *filename)
{
    int len = strnlen(filename, Kernel16F_MAX_PATH);
    return len >= 3 && isdigit(filename[0]) && memcmp(&filename[1], ":/", 2) == 0;
}

static int fs_get_drive_by_path(char *filename)
{
    int len = strnlen(filename, Kernel16F_MAX_PATH);
    if (!fs_valid_path_format(filename))
    {
        return -Kernel16F_BAD_PATH;
    }

    return tonumericdigit(filename[0]);
}

int fopen(char *filename, char mode)
{
    int drive_no = fs_get_drive_by_path(filename);
    if (drive_no < 0)
    {
        return drive_no;
    }

    char* start_of_relative_path = &filename[2];
    struct disk *disk = disk_get(drive_no);
    if (!disk)
    {
        return -Kernel16F_INVALID_DRIVE;
    }

    return filesystems[0]->open(disk, start_of_relative_path, mode);
}

struct filesystem *fs_resolve(struct disk *disk)
{
    for (int i = 0; i < MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != NULL && filesystems[i]->resolve(disk) == 0)
        {
            return filesystems[i];
        }
    }

    return NULL;
}
