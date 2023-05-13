[org 0x7c00]

KERNEL_OFFSET equ 0x1000    ; This is the memory offset to which we will load our kernel

    mov [BOOT_DRIVE], dl    ; BIOS stores our boot drive in DL, so it’s
                            ; best to remember this for later.


    ; setting up stack
    mov bp, 0x9000  
    mov sp, bp

    mov bx, GREETING_REAL_MODE
    call print_string

    call load_kernel

    call switch_to_pm

    jmp $       ; <-- we should never actually get to this instruction



%include "./boot/print_string.asm"
%include "./boot/disk_load.asm"
%include "./boot/gdt.asm"
%include "./boot/switch_to_pm.asm"
%include "./boot/print_string_32.asm"


[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET   ; where to write loaded data to
    mov dh, 15              ; nr of sectors to load (15 is too much, but just to be sure)
    mov dl, [BOOT_DRIVE]    ; drive from which to load (is dl by default, anyway, but just making sure that's not changed)
    call disk_load
    ret


[bits 32]

BEGIN_PM:
    mov ebx, GREETING_PROTECTED_MODE
    call print_string_pm

    call KERNEL_OFFSET

    loop:
        jmp loop



; global variables
BOOT_DRIVE:
    db 0
GREETING_REAL_MODE:
    db "Hello from real mode!",0
GREETING_PROTECTED_MODE:
    db "Hello from protected mode!",0


times 510 - ($ - $$) db 0
dw 0xaa55
