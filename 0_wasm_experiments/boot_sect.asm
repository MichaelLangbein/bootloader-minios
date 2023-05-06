db 'ZYX'

mov ax, 0x7c00
mov cs, ax

mov ah, 0x0e
; mov al, [0x7c00]
mov al, [cs:0x01]
int 0x10

; mov bx, 0x7c00
; call print_string

;mov dx, 0x7c01
;call print_hex

loop:
    jmp loop

%include "print_string.asm"
%include "print_hex.asm"

times 510 - ($ - $$) db 0
dw 0xaa55