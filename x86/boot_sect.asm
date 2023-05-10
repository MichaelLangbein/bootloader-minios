mov bx, 0x7c00
add bx, HEX_OUT
call print_string

loop:
    jmp loop

to_char:
    cmp cx, 0xa
    jl digits
    sub cx, 0xa
    add cx, 'a'
    ret
    
digits:
    add cx, '0'
    ret
    
HEX_OUT:
    db 'The string - which is pretty long and exceeds 8 bits',0


%include "./print_string.asm"

times 510 - ($ - $$) db 0
dw 0xaa55