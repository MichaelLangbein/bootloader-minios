[org 0x7c00]

mov bx, greeting        ; this works with bx, but not with bh
printString:
    mov ah, 0x0e
    mov al, [bx]
    cmp al, 0
    je loop
    int 0x10
    add bx, 1
    jmp printString

loop:
    jmp loop

greeting:
    db 'Hello, world!',0

times 510 - ($ - $$) db 0
dw 0xaa55
