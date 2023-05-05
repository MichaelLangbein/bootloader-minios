[org 0x7c00]

mov bx, greeting        ; this works with bx, but not with bh
call printString

mov bx, goodbye
call printString

loop:
    jmp loop

printString:
    pusha
    mov ah, 0x0e
    printChar:
        mov al, [bx]
        cmp al, 0
        je exitPrintString
        int 0x10
        add bx, 1
        jmp printChar
    exitPrintString:
        popa
        ret

greeting:
    db 'Hello, world!',0

goodbye:
    db 'Goodbye!',0

times 510 - ($ - $$) db 0
dw 0xaa55
