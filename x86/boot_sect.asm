[org 0x7c00]

    mov bp, 0x9000
    mov sp, bp

    mov bx, GREETING_REAL_MODE
    call print_string

    call switch_to_pm

    mov ebx, GREETING_PROTECTED_MODE
    call print_string_pm

    jmp $

%include "./print_string.asm"
%include "./gdt.asm"
%include "./print_string_32.asm"
%include "switch_to_pm.asm"

GREETING_REAL_MODE:
    db "Hello from real mode!",0

[bits 32]
GREETING_PROTECTED_MODE:
    db "Hello from protected mode!",0