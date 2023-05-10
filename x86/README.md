# Assembly

## Operations
Maps directly to machine instruction:
- `mov <destination-register>, <value>`: put value into destination-register
- `mov <destination-register>, <other register>`: puts value stored at `other register` into destination
- `mov <destination-register>, [<other register>]`: puts value pointed at by adress stored in `other register` into destination

- `db <byte-value>`: write a byte
- `dw <word-value>`: write a word (== 2 bytes in 16-bit mode)

- `int <interupt-number>`: message to bios: run interupt-procedure with id `interupt-number`

- `jmp <offset>`
- `cmp <value or register> <value or register>; je <offset>`: jump if comparison results in `equal`

- `add <to-register>, <value>`

- `push <value>`
- `pop <into-target-register>`

- `pusha`
- `popa`
- `ret`
- `call`



Higher level syntax:
- `$`: current address
- `$$`: address of start of current section 
- `times <number> <instruction>`
- `<label>:
    <instructions for label>`: returns offset from $$
        - to make this label resolvable with `[label]`, first add `0x7c00` (==offset of bootloader code from 0x0)
- `%include "filename.asm"`


- Register addressing
    - `mov eax, ebx`: Copies what is in ebx into eax
    - `mov esi, var`: Copies address of var (say 0x0040120e) into esi

- Immediate addressing (second operand is an immediate constant)
    - `mov bx, 20`: 16-bit register bx gets the actual value 20

- Direct memory addressing (directly loads from memory through a specified address)
    - `mov ax, [1000h]`: loads a 2-byte object from the byte at address 4096 (0x1000 in hexadecimal) into a 16-bit register called 'ax'
    - `mov [1000h], ax`: memory at address 1000h gets the value of ax

- Direct offset addressing (same as 3, just using arithmetics to modify address)
    - `mov al, [byte_tbl+2]`
    
- Register indirect (accessing memory by using addresses stored in registers)
    - `mov ax, [di]`: copies value at memory address specified by di, into ax
    - `mov dword [eax], var1`: copies value in var1 into the memory slot specified by eax



## Registers

General purpose:

| Code | Name           | Purpose                              | Notes                                     |
|------|----------------|--------------------------------------|-------------------------------------------|
| `ax` | accumulator    | arithmatics                          |                                           |
| `cx` | counter        | arithmatics, I/O                     |                                           |
| `dx` | data           | arithmatics, I/O                     |                                           |
| `bx` | base           | pointer to data, together with `ds`. | allows indirect adressing: `mov ax, [bx]` |
| `sp` | stac-top       | pointer to top of stack              |                                           |
| `bp` | stac-base      | pointer to bottom of stack           | allows indirect adressing: `mov ax, [bp]` |
| `si` | stream-source  | streams                              | allows indirect adressing: `mov ax, [si]` |
| `di` | stream-destina | streams                              | allows indirect adressing: `mov ax, [di]` |


Segment-registers:

All segment-registers must be set indirectly:
```asm
mov cx, 0x7c0
mov ds, cx
```

| Code | Purpose                | Usual value  | Notes      |
|------|------------------------|--------------|------------|
| `ss` | stack                  |              |            |
| `cs` | code                   | 0x7c00       | Can't be overwritten, I think      |
| `ds` | data                   |              | Is implicitly accessed with `[offset]`, but `[ds:offset]` works, too  |
| `es` | extra data             |              | `[es:offset]`  |
| `fs` | more extra data        |              | `[fs:offset]`  |
| `gs` | yet more extra data    |              | `[gs:offset]`  |



# Examples

## Print
```nasm
mov ah, 0x0e
mov al, 'A'
int 0x10
```

## Print as function
```asm
; Naive printing: put literal into al
; mov ah, 0x0e
; mov al, 'A'
; int 0x10

; function printing: put address into bx
mov bx, 0x7c00
add bx, HEX_OUT
call print_string

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
```

## Offsets
```nasm
[org 0x7c00]

mov ah, 0x0e
mov bh, [first]
add bh, 2
mov al, bh
int 0x10

loop:
    jmp loop

first:
    db 'A'

second:
    db 'B'

third: 
    db 'C'
```

## Proof that code starts at 0x7c00
```asm
db 'Z'
db 'Y'

mov ah, 0x0e
mov al, [0x7c01]
int 0x10

loop:
    jmp loop

times 510 - ($ - $$) db 0
dw 0xaa55
```