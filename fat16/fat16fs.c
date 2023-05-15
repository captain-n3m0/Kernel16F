#include "../headers/stdio.h"
#include "fat16fs.h"
#include "../memory_manager/mem_manager.h"
#include "../string/string.h"
#include "../status.h"
#include "../config.h"
#include "../disk_operations/disk.h"
#include "../headers/stdint.h"
#include "../headers/stddef.h"

struct fat_header_extended
{
    uint8 drive_number;
    uint8 win_nt_bit;
    uint8 signature;
    uint32 volume_id;
    uint8 volume_id_string[11];
    uint8 system_id_string[8];
};

struct fat_header
{
    uint8 oem_identifier[8];
    uint16 bytes_per_sector;
    uint8 sectors_per_cluster;
    uint16 reserved_sectors;
    uint8 fat_copies;
    uint16 root_dir_entries;
    uint16 number_of_sectors;
    uint8 media_type;
    uint16 sectors_per_fat;
    uint16 sectors_per_track;
    uint16 number_of_heads;
    uint32 hidden_sectors;
    uint32 sectors_big;
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
static int fat16_resolve(struct disk *disk)
{
    int res = 0;
    struct fat_private *fat_private = kmalloc(sizeof(struct fat_private));
    //print_number(fat_private);
    //print("testing");

    if (!disk_read_block(disk, 0, &fat_private->header))
    {
        res = -KERNEL16F_FS_NOT_US;
        goto out;
    }


    {
        struct fat_h *header = &fat_private->header;
        if (header->shared.extended_header.signature != KERNEL16F_FAT16_SIGNATURE)
        {

            res = -KERNEL16F_FS_NOT_US;
            goto out;
        }
    }
out:
    disk->fs_private = fat_private;
    if (res < 0)
    {
        kfree(fat_private);
        disk->fs_private = 0;
    }

    return res;
}

static int fat16_get_root_directory(struct disk *disk)
{
    struct fat_private* private = (struct fat_private*) disk->fs_private;
    struct fat_header* primary_header = &private->header.primary_header;
    //int root_dir_sector_pos = (primary_header->fat_copies * primary_header->sectors_per_fat)  + primary_header->reserved_sectors;
       // print(private->header.primary_header.oem_identifier);

    print(primary_header->oem_identifier);
   //print_number(root_dir_sector_pos);

    return 0;
}

static void *fat16_open(struct disk *disk, char *filename, char mode)
{


    // Read only filesystem
    if (mode != 'r')
    {
        return -KERNEL16F_FS_FILE_READ_ONLY_ERROR;
    }

fat16_get_root_directory(disk);

    return KERNEL16F_ALL_OK;
}

struct filesystem *fat16_init()
{
    fat16_fs.open = fat16_open;
    fat16_fs.resolve = fat16_resolve;
    strncpy(fat16_fs.name, "FAT16", sizeof(fat16_fs.name));
    return &fat16_fs;
}
