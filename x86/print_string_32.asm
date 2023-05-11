[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f


; prints string at pointer in ebx:
; mov ebx, <string-address>
; uses edx as working-register.
print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY       ; load beginning of MMIO-video-memory into edx

print_string_pm_loop:
                                ; filling ax with char-info
    move al, [ebx]              ; fist 8 bits: ascii-code
    mov ah, WHITE_ON_BLACK      ; last 8 bits: styling

    cmp al, 0
    je print_string_pm_done

    mov [edx], ax               ; put content of ax into MMIO-video-memory
    add ebx, 1                  ; point to next letter in string
    add edx, 2                  ; point to next entry in MMIO-video-memory

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret