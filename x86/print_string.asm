
; strangely, this function works with bx, but not with bh
print_string:
    pusha
    mov ah, 0x0e
    printChar:
        mov al, [bx]
        cmp al, 0
        je exitprint_string
        int 0x10
        add bx, 1
        jmp printChar
    exitprint_string:
        popa
        ret