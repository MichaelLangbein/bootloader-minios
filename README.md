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
- `[label-offset]`: fetch value at offset
- `[register-name]`: fetch value from the address stored in `register-name`
- `[org 0x7c00]`: tells assember offset of code. This way, adding 0x7c00 is no longer required.
- `%include "filename.asm"`


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

| Code | Purpose                | Usual value  |
|------|------------------------|--------------|
| `ss` | stack                  |              |
| `cd` | code                   | 0x7c00       |
| `ds` | data                   |              |
| `es` | extra data             |              |
| `fs` | more extra data        |              |
| `gs` | yet more extra data    |              |



# Examples

## Print
```nasm
mov ah, 0x0e
mov al, 'A'
int 0x10
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