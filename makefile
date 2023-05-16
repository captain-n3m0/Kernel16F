AS=nasm
CC=bcc
LD=ld86
INC=./src
SRC=./src
OBJECTS= ./build/Kernel16F.o ./build/disk/disk.o ./build/disk/diskasm.o ./build/display.o ./build/displayasm.o ./build/memory/memory.o ./build/memory/heap.o ./build/memory/kheap.o ./build/string/string.o  ./build/fs/file.o ./build/fs/fat/fat16.o
INC=./src
CFLAGS=-0 -I$(INC) -ansi -c

all: ./bin/boot.bin ./bin/Kernel16F.bin
	rm -f ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/Kernel16F.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/Kernel16F.o: ./src/Kernel16F.c ./src/Kernel16F.h
	$(CC) $(CFLAGS) $(SRC)/Kernel16F.c -o ./build/Kernel16F.o

./build/disk/disk.o: ./src/disk_operations/disk.c ./src/disk_operations/disk.h
	$(CC) $(CFLAGS) -I./src/disk_operations $(SRC)/disk_operations/disk.c -o ./build/disk_operations/disk.o

./build/disk_operations/diskasm.o: ./src/disk_operations/disk.asm
	nasm -f as86 ./src/disk_operations/disk.asm -o ./build/disk_operations/diskasm.o

./build/display.o: ./src/display/display.c ./src/display/display.h
	$(CC) $(CFLAGS) $(SRC)/display.c -o ./build/display.o

./build/displayasm.o: ./src/display/display.asm
	nasm -f as86 ./src/display/display.asm -o ./build/displayasm.o

./build/memory_manager/mem_manager.o: ./src/memory_manager/mem_manager.c ./src/memory_manager/mem_manager.h
	$(CC) $(CFLAGS) -I./src/memory_manager $(SRC)/memory_manager/mem_manager.c -o ./build/memory_manager/mem_manager.o

./build/string/string.o: ./src/string/string.c ./src/string/string.h
	$(CC) $(CFLAGS) -I./src/string $(SRC)/string/string.c -o ./build/string/string.o

./build/filesystem/filesystem.o: ./src/filesystem/filesystem.c ./src/filesystem/filesystem.h
	$(CC) $(CFLAGS) -I./src/filesystem $(SRC)/filesystem/filesystem.c -o ./build/filesystem/filesystem.o

./build/fat16/fat16fs.o: ./src/fat16/fat16fs.c ./src/fat16/fat16fs.h
	$(CC) $(CFLAGS) -I./src/fat16 $(SRC)/fat16/fat16fs.c -o ./build/fat16/fat16fs.o

./bin/Kernel16F.bin: ${OBJECTS}
	$(LD) -d -M ${OBJECTS} -L/usr/lib/bcc/ -lc -o ./bin/Kernel16F.bin

clean:
	rm -f ./bin/boot.bin
	rm -f ./bin/Kernel16F.bin
	rm -f ./bin/os.bin
	rm -f ${OBJECTS}

run:
