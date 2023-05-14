# Compiler and linker options
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -Wall -Wextra -Werror -Iinclude
LDFLAGS = -e _start

# Source files
SRCS = Kernel16F.c display/display.c filesystem/filesystem.c disk_operations/disk.c memory_manager/mem_manager.c string/string.c process_manager/proc_man.c cli_interface/cli.c

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
