%include "constants.asm"
[BITS 16]
global _disk_reset
global _disk_read_sector
global _disk_get_disk_details
extern _print_number

_disk_reset:
    push bp
    mov bp, sp
    push di
    push si

    ; Reset disk using BIOS interrupt 0x13
    mov drive_number, [bp+4]
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
    mov ah, 0x02
    mov total_sectors, [bp+14]
    mov cylinder, [bp+12]
    mov sector, [bp+10]
    mov head, [bp+8]
    mov drive_number, [bp+6]
    mov destination, [bp+4]
    int 0x13

    ; Check carry flag to see if there was an error
    mov success, 0x01
    jc .error
    jmp .done

.error:
    mov success, 0x00

.done:
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
    mov drive_number, [bp+4]
    mov cx, 0
    mov ah, 0x08
    int 0x13

    ; Check carry flag to see if there was an error
    jc .error

    ; Extract number of heads and sectors per track from DX and CL registers
    shr number_of_sectors, 7
    add number_of_sectors, 1
    and number_of_heads, 0x3f

    ; Store number of heads and sectors per track in memory
    mov bx, [bp+6] ; &NumberOfHeads
    mov word [bx], number_of_heads
    mov bx, [bp+8] ; &NumberOfSectorsPerTrack
    mov word [bx], number_of_sectors

    ; Set return value to 1 to indicate success
    mov success, 0x01
    jmp .done

.error:
    mov success, 0x00

.done:
    ; Debugging code to print number of heads
    mov bx, [bp+6]
    push word [bx]
    ;call _print_number
    pop bx

    pop si
    pop di
    pop bp
    ret
