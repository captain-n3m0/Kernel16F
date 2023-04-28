[BITS 16]   ; Set the code generation to 16-bit mode

; Define constants
KERNEL_LOAD_ADDRESS equ 0x1000   ; The address where the kernel will be loaded
SECTOR_SIZE         equ 512     ; The size of a disk sector in bytes

; Define data segments
section .data
    ; Define the boot message
    boot_message db "Loading kernel...", 0

section .text
    ; Define the bootloader entry point
    global _start
    _start:
        ; Print the boot message
        mov ah, 0x0E     ; BIOS interrupt to print a character to the screen
        mov bh, 0x00     ; Video page
        mov bl, 0x07     ; Text attribute (white on black)
        mov si, boot_message
        call print_string

        ; Load the kernel from disk to memory
        mov ah, 0x02     ; BIOS interrupt to read a disk sector
        mov al, 0x01     ; Number of sectors to read
        mov ch, 0x00     ; Cylinder number (first cylinder)
        mov cl, 0x02     ; Sector number (third sector)
        mov dh, 0x00     ; Head number (first head)
        mov dl, 0x80     ; Disk number (boot device)
        mov bx, KERNEL_LOAD_ADDRESS
        int 0x13         ; Call BIOS disk interrupt

        ; Jump to the kernel's entry point
        jmp KERNEL_LOAD_ADDRESS

; Define functions
print_string:
    ; Print a null-terminated string to the screen
    mov ah, 0x0E     ; BIOS interrupt to print a character to the screen
    mov bh, 0x00     ; Video page
    mov bl, 0x07     ; Text attribute (white on black)
    .loop:
        lodsb         ; Load a byte from [si] into al, and increment si
        cmp al, 0     ; Check if the byte is null
        je .done      ; If it is, exit the loop
        int 0x10      ; Otherwise, print the character
        jmp .loop     ; And continue the loop
    .done:
        ret           ; Return from the function
