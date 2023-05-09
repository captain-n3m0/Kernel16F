#include "disk.h"
#include "config.h"
#include "memory_manager.h"
#include "kernel.h"
#include "memory/kheap.h"

static struct disk disks[Kernel16F_MAX_DISKS];
static int num_disks = 0;

extern int disk_reset(int drive_number);
extern int disk_read_sector(void *dest, int drive_number, int head, int sector, int cylinder, int total_sectors);

static void locate_disk(int bios_drive_id)
{
    if (disk_reset(bios_drive_id))
    {
        // Has to be static due to compiler bug issue with passing addresses to functions
        static int sectors_per_track = 0;
        static int total_heads = 0;

        // Ok this disk exists lets set it all up
        struct disk *disk = &disks[0];
        disk->type = Kernel16F_DISK_TYPE_REAL;
        disk->shared.real.bios_id = bios_drive_id;

        if (disk_get_details(bios_drive_id, &total_heads, &sectors_per_track))
        {
            disk->shared.real.total_heads = total_heads;
            disk->shared.real.sectors_per_track = sectors_per_track;
        }
        disk->filesystem = fs_resolve(disk);
    }
}

static void locate_all_disks()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        locate_disk(0x80 + i);
    }
}

void disk_search_and_init()
{
    num_disks = 0;
    memset(disks, 0, sizeof(disks));
    locate_all_disks();
}

static int is_disk_registered(int index)
{
    struct disk empty_disk;
    memset(&empty_disk, 0, sizeof(empty_disk));
    return index < Kernel16F_MAX_DISKS &&
           memcmp(&disks[index], &empty_disk, sizeof(disks[index])) != 0;
}

struct disk *get_disk(int index)
{
    if (!is_disk_registered(index))
        return 0;

    return &disks[0];
}

int read_disk_block(struct disk *disk, int lba, char *buf)
{
    struct real_disk *rdisk = &disk->shared.real;
    int head = (lba % (rdisk->sectors_per_track * 2) / rdisk->sectors_per_track);
    int track = (lba / (rdisk->sectors_per_track * 2));
    int sector = (lba % rdisk->sectors_per_track + 1);

    return disk_read_sector(buf, rdisk->bios_id, head, sector, track, 1);
}
