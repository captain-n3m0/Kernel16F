# Compiler and linker options
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra -Werror
LDFLAGS = -m elf_i386

# Source files and object files
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Kernel output file
KERNEL = kernel.elf

# Build rules
all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
