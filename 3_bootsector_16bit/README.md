# ASM

registers:
- `ax, ah, al`
- `bx, bh, bl`
- `cx, ch, cl`
- `dx, dh, dl`
- `bp`: address of stack-base
- `sp`: address of stack-top

Maps directly to machine instruction:
- `db <byte-value>`: write a byte
- `dw <word-value>`: write a word (== 2 bytes in 16-bit mode)
- `mov <destination-register>, <value>`: put source into destination-register
- `int <interupt-number>`: message to bios: run interupt-procedure with id `interupt-number`
- `jmp <offset>`
- `add <to-register>, <value>`
- `push <value>`
- `pop <into-target-register>`



Higher level syntax:
- `$`: current address
- `$$`: address of start of current section 
- `times <number> <instruction>`
- `<label>:
    <instructions for label>`: returns offset from $$
        - to make this label resolvable with `[label]`, first add 0x7c00 (==offset of bootloader code from 0x0)
- `[label-offset or register-name]`: fetch value at offset
- `[org 0x7c00]`: tells assember offset of code. This way, adding 0x7c00 is no longer required.
