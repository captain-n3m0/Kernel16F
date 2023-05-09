[BITS 16]
global _putchar
; Print a single character to the screen
; Input: al = character to print
; Clobbers: ax, bx, cx, dx
_putchar:
push bp
mov bp, sp
pusha
mov ah, 0x0e ; BIOS function code for "print character"
int 0x10     ; invoke BIOS function

popa
pop bp
ret
