#include "fat16fs.h"
#include "memory_manager/mem_manager.h"
#include "string_manipulation/string_utils.h"
#include "status.h"
#include "config.h"
#include "disk_operations/disk.h"

struct fat_header_extended
{
    uint8_t drive_number;
    uint8_t win_nt_bit;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_id_string[11];
    uint8_t system_id_string[8];
};

struct fat_header
{
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t sectors_big;
};

struct fat_h
{
    struct fat_header primary_header;
    union fat_h_e {
        struct fat_header_extended extended_header;
    } shared;
};

struct fat_private
{
    struct fat_h header;
};

static struct filesystem fat16_fs;

static int fat16_resolve(struct disk* disk)
{
    int res = 0;
    struct fat_private* fat_private = kmalloc(sizeof(struct fat_private));

    if (!disk_read_block(disk, 0, &fat_private->header))
    {
        res = -Kernel16F_FS_NOT_US;
        goto out;
    }

    struct fat_h* header = &fat_private->header;
    if (header->shared.extended_header.signature != Kernel16F_FAT16_SIGNATURE)
    {
        res = -Kernel16F_FS_NOT_US;
        goto out;
    }

out:
    disk->fs_private = fat_private;
    if (res < 0)
    {
        kfree(fat_private);
        disk->fs_private = NULL;
    }

    return res;
}

static int fat16_get_root_directory(struct disk* disk)
{
    struct fat_private* private = (struct fat_private*)disk->fs_private;
    struct fat_header* primary_header = &private->header.primary_header;

    print("OEM Identifier: ");
    print((char*)primary_header->oem_identifier);
    print("\n");

    return 0;
}

static void* fat16_open(struct disk* disk, char* filename, char mode)
{
    // Read-only filesystem
    if (mode != 'r')
    {
        return (void*)-Kernel16F_FS_FILE_READ_ONLY_ERROR;
    }

    fat16_get_root_directory(disk);

    return (void*)Kernel16F_ALL_OK;
}

struct filesystem* fat16_init()
{
    fat16_fs.open = fat16_open;
    fat16_fs.resolve = fat16_resolve;
    strncpy(fat16_fs.name, "FAT16", sizeof(fat16_fs.name));
    return &fat16_fs;
}
