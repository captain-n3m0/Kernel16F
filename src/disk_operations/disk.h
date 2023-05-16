#ifndef DISK_H
#define DISK_H

#include "../filesystem/filesystem.h"

typedef enum {
    KERNEL16F_DISK_TYPE_REAL = 0,
} KERNEL16F_DISK_TYPE;

struct real_disk {
    int bios_id;
    int total_heads;
    int sectors_per_track;
};

struct disk {
    KERNEL16F_DISK_TYPE type;
    struct filesystem *filesystem;
    union {
        struct real_disk real;
    } shared;
    void *fs_private;
};

void disk_search_and_init(void);
struct disk *disk_get(int index);
int disk_read_block(struct disk *disk, int lba, char *buf);
int disk_get_details(int bios_disk_id, int *total_heads, int *sectors_per_track);

#endif
