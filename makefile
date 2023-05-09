# Compiler and linker options
CC = gcc
LD = ld
CFLAGS = -m16 -ffreestanding -Wall -Wextra -Werror -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS = -T Kernel16F.ld -melf_i386

# Source files
SRCS = Kernel16F.c display.c filesystem/filesystem.c disk_operations/disk.c memory_manager/mem_manager.c string_manipulation/string_utils.c process_manager/proc_man.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output file
OUT = Kernel16F.elf

# Build rules
all: $(OUT)

$(OUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(OUT)
