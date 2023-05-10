; Naive printing: put literal into al
; mov ah, 0x0e
; mov al, 'A'
; int 0x10

; function printing: put address into bx
; mov bx, 0x7c00
; add bx, HEX_OUT
; call print_string
print_string:
    mov al, [bx]
    cmp al, 0
    je exit

    mov ah, 0x0e
    int 0x10
    add bx, 1
    call print_string

    exit:
        ret