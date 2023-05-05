[org 0x7c00]

mov ah, 0x0e
mov bl, [greeting]
jmp printChar


loop:
    jmp loop

printChar:
    mov al, bl
    cmp al, 0
    je loop
    int 0x10
    add bl, 1
    jne printChar

first:
    db 'A'

second:
    db 'B'

third: 
    db 'C'

greeting:
    db "Welcome to Mykonos",0


times 510 - ($ - $$) db 0
dw 0xaa55
