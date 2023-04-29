BITS 16

start:
    ; Set up the segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Load the kernel from disk
    mov bx, kernel_start
    mov dh, 1
    mov dl, [boot_drive]
    mov ah, 2
    mov al, dh
    mov ch, 0
    mov cl, 2
    int 0x13
    jc boot_error

    ; Jump to the kernel
    jmp dword [kernel_start]

boot_error:
    ; Print an error message and halt the system
    mov si, boot_error_msg
    call print_string
    cli
    hlt

print_string:
    ; Print a null-terminated string
    pusha
    mov ah, 0x0e
.repeat:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .repeat
.done:
    popa
    ret

boot_drive: db 0
boot_error_msg: db "Error loading kernel!", 0

; The kernel is loaded at this address by the linker
kernel_start: dd 0x8000
