[BITS 16]

; Set up the segment registers
xor ax, ax
mov ds, ax
mov es, ax

; Load the kernel into memory
mov bx, 0x1000
mov ah, 0x02
mov al, 0x01
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
mov dl, 0x80
int 0x13

; Jump to the kernel
jmp 0x1000

; Pad the bootloader to 510 bytes
times 510-($-$$) db 0

; Add the boot signature
dw 0xAA55
