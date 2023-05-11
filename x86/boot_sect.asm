[org 0x7c00]

    mov bp, 0x9000
    mov sp, bp

    mov bx, GREETING_REAL_MODE
    call print_string

    call switch_to_pm

GREETING_REAL_MODE:
    db "Hello from real mode!",0

%include "./print_string.asm"
%include "./gdt.asm"
%include "switch_to_pm.asm"
%include "./print_string_32.asm"




[bits 32]

BEGIN_PM:
    mov ebx, GREETING_PROTECTED_MODE
    call print_string_pm

    loop:
        jmp loop


GREETING_PROTECTED_MODE:
    db "Hello from protected mode!",0


times 510 - ($ - $$) db 0
dw 0xaa55
