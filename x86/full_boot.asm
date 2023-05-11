
loop:
    loop

greeting:
    db 'Hello, os!',0

%include "./print_string_32.asm"

times 510 - ($ - $$) db 0
dw 0xaa55


times 256 dw 'Aa'
times 256 dw 'Bb'