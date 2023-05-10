; stores read sectors in es:bx 
; mov ah, 0x02        ; read sector function
; mov al, 0x01        ; read 1 sector
; mov ch, 0x00        ; on cylinder 0
; mov dh, 0x00        ; with head 0
; mov cl, 0x02        ; start reading from second sector
; int 0x13


; Task: experiment with sector-registers and offsets.
;
; Obs 1: 
; 
; this works:
; mov bx, 0x7c00
; add bx, str
; call print_string
; str:
;     db "This is a string",0
; 
; This doesn't:
; str:
;     db "This is a string",0
; mov bx, 0x7c00
; add bx, str
; call print_string
; 
; Why?


; works:
; mov bx, 0x7c00
; add bx, str
; call print_string

; doesn't work
; mov cx, 0x7c0
; mov es, cx
; mov bx, [es:str] <-- reason: this fetches the value at [es:str], but we need the address
; call print_string 

; works
; mov cx, 0x7c0
; mov es, cx
; mov ah, 0x0e
; mov al, [es:str]
; int 0x10


mov ah, 0x0e
mov al, [es:str]
int 0x10

; mov cx, 0x7c0
; mov es, cx
; mov bx, [es:str]
; call print_string_val 

loop:
    jmp loop

str:
    db "This is a string",0


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


times 510 - ($ - $$) db 0
dw 0xaa55