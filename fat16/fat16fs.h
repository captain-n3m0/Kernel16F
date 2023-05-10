#include "../filesystem/filesystem.h"
#include "../types.h"


#define KERNEL16F_FAT16_SIGNATURE 0x29
struct filesystem* fat16_init();
