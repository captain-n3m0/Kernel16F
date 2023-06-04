[BITS 16]
global _disk_reset
global _disk_read_sector
global _disk_get_disk_details
extern _print_number
drive_number equ 0x80
success equ 0x00
total_sectors equ x0C
cylinder equ 0x00
sector equ 0x02
head equ 0x00
destination equ 0x7E00
number_sectors equ 0x01
number_of_heads equ 0x01

_disk_reset:
    push bp
    mov bp, sp
    push di
    push si

    ; Reset disk using BIOS interrupt 0x13
    mov dl, [bp+4]
    mov ah, 0x00
    int 0x13

    ; Check carry flag to see if there was an error
    mov success, 0x01
    jc .error
    jmp .end

.error:
    mov success, 0x00

.end:
    pop si
    pop di
    pop bp
    ret

_disk_read_sector:
    push bp
    mov bp, sp
    push di
    push si

    ; Read sector using BIOS interrupt 0x13
    mov ah, x02
    mov cylinder, [bp+12]
    mov sector, [bp+10]
    mov head, [bp+8]
    mov dl, drive_number
    mov bx, [bp+4]
    mov es, bx
    mov bx, [bx+2]
    mov destination, bx
    mov al, number_sectors
    int 0x13

    ; Check carry flag to see if there was an error
    mov success, 0x01
    jc .error
    jmp .end

.error:
    mov success, 0x00

.end:
    pop si
    pop di
    pop bp
    ret

_disk_get_disk_details:
    push bp
    mov bp, sp
    push di
    push si

    ; Get disk details using BIOS interrupt 0x13
    mov dl, [bp+4]
    mov cx, 0
    mov ah, 0x08
    int 0x13

    ; Check carry flag to see if there was an error
    jc .error

    ; Extract number of heads and sectors per track from DX and CL registers
    shr cx, 7
    add cx, 1
    and dx, 0x3f

    ; Store number of heads and sectors per track in memory
    mov bx, [bp+12] ; &NumberOfHeads
    mov word [bx], dx
    mov bx, [bp+10] ; &NumberOfSectorsPerTrack
    mov word [bx], cx

    ; Set return value to 1 to indicate success
    mov success, 0x01
    jmp .done

.error:
    mov success, 0x00

.done:
    ; Debugging code to print number of heads
    mov bx, [bp+12]
    push word [bx]
    call _print_number
    pop bx

    pop si
    pop di
    pop bp
    ret

_print_number:
    push bp
    mov bp, sp
    push bx

    ; Print number using BIOS interrupt 0x10
    mov bx, [bp+6]
 mov cx, 0
    mov ax, 0x0E
    mov bl, 0x07
    shr bx, 4
    call .print_digit
    mov bl, 0x0F
    and bx, 0x0F
    call .print_digit

    pop
    pop bp
    ret

.print_digit:
    cmp bx, 0x0A
    jl .less_than_10
    add bx, 0x07
    .less_than_10:
    add bx, 0x30
    mov ah, 0x0E
    int 0x10
    ret
