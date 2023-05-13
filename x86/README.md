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
| `fs` | more extra data        |              | `[fs:offset]`; only in 32bit mode  |
| `gs` | yet more extra data    |              | `[gs:offset]`; only in 32bit mode  |

Note that `[es:offset]` get's us the value at `es * 16 + offset`.
If instead we want the address, we'd write 
```asm
mov cx,  0x7c00  ; note the extra 0 at end
add cx, <offset>
```



# Switching to 32 bits

## Printing
- Display already known to bios.
- Starts in VGA-compatability mode.
- Memory-mapped IO.
- Has graphics mode and text-mode.
- Text-mode:
    - 80 * 25 character-cells.
    - Already has some simple font in internal memory of VGA display device.
    - Each cell's letter defined by 16 bits: fist 8: ascii-code, last 8: color, blinking, etc.
    - Usually starts at 0xb8000

## GDT

- List of segment-descriptors
- Segment-descriptor: 64-bit structure
    - Base-address: 32-bit
    - Segment-limit: 20-bits: size of segment
    - Flags: 12-bits. (Privilege level, read-only, write-only, ...)

layout:
| bit 0-15       | bit 16-31   | 32-39       | 40-49      | 50-53          | 54-        | 57-63 |
|----------------|-------------|-------------|------------|----------------|------------|--|
| segment part 1 | base part 1 | pase part 2 | flags      | segment part 2 | flags      | base part 3 |


Simplest configuration: *basic flat model*.
- Initial null-descriptor: 8 zero-bytes.
- Two overlapping segments: *code* and *data*.
- Cover full 4GB.
- Overlapping: no protection of one segment from the other. No paging for virtual memory, either.
- Code segment:
    - base: 0x0
    - limit: 0xffffff
    - present: 1 (a virt-mem-setting, 1 means "really in mem, not only virtual")
    - privilege: 0 (highest ring)
    - descriptor type: 1 (code or data, 0 is for traps)
    - Type:
        - Code: 1
        - Conforming: 0 (code in lower-privilege-segments must not call code in this segment)
        - Readable: 1 (0 = exec only)
        - Accessed: 0
    - Some more flags:
        - Granularity: 1 (multiplies limit by 4k => limit becomes from 0xffffff to 0xffffff000 = 4GB)
        - 32bit default: 1
        - 64bit code segment: 0
        - AVL: 0 (sometimes used for debugging)
- Data segment:
    - identical (because overlapping), except:
    - Type:
        - Code: 0 (data)
        - Expand down: 0
        - Writable: 1
        - Accessed: 0

Additionally: 
GDT-descriptor (48 bits)
    - GDT size (16 bits)
    - GDT address (32 bits)

    


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

## Loading from disk
```asm
; load DH sectors to ES:BX from drive DL
disk_load:
    push dx         ; Store DX on stack so later we can recall
                    ; how many sectors were request to be read ,
                    ; even if it is altered in the meantime
    mov ah , 0x02   ; BIOS read sector function
    mov al , dh     ; Read DH sectors
    mov ch , 0x00   ; Select cylinder 0
    mov dh , 0x00   ; Select head 0
    mov cl , 0x02   ; Start reading from second sector ( i.e.
                    ; after the boot sector )
    int 0x13        ; BIOS interrupt

    ; bios will save results in es:bx = 16 * 0xa000 + 0x1234 = 0xa1234
    ; mov bx, 0xa000
    ; mov es, bx
    ; mov bx, 0x1234


    jc disk_error   ; Jump if error ( i.e. carry flag set )
    pop dx          ; Restore DX from the stack
    cmp dh , al     ; if AL ( sectors read ) != DH ( sectors expected )
    jne disk_error  ; display error message
    ret
    
disk_error :
    mov bx , DISK_ERROR_MSG
    call print_string
    jmp $
    
; Variables
DISK_ERROR_MSG db " Disk read error !", 0
```



# Open questions
```asm
; this works:
mov bx, 0x7c00
add bx, str
call print_string
str:
    db "This is a string",0
 
; This doesn't:
str:
    db "This is a string",0
mov bx, 0x7c00
add bx, str
call print_string

; Why?

```


# I/O
Talking to other devices
- CPU is connected to controllers
- The controllers talk to devices
- CPU talks to controllers by reading and setting values to their registers
- These registers are mapped to a separate section of memory
- Often, CPU doesn't actually talk to a device-specific controller, but to a bus-controller, which then talks to a device-controller at a rate that fits the device (this way the CPU needs not slow down to the device-speed)

Example of writing to a device-controller, using the instructions `in` and `out`
```asm
; we know that floppy-controller is mapped to I/O memory location 0x3f2
mov dx, 0x3f2       ; save into dx the contents of the device-controller
in al, dx           ; read contents into al
or al, 00010000b    ; set 4th bit of contents to true to start floppy-motor.
out dx, al          ; send updated contents back to controller
```
