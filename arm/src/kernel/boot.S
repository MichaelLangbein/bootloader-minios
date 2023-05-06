.section ".text.boot"   // note to linker: where to place this in final binary

.global _start          // note to linker: export `_start` to outside

_start:
    mrc p15, #0, r1, c0, c0, #5     // use only one of four cores
    and r1, r1, #3                  
    cmp r1, #0          // check if processor nr 0 ...
    bne halt            // ... if not, jump to `halt`

    mov sp, #0x8000                 // c-stack starts at 0x8000

    ldr r4, =__bss_start            // bss: block-starting symbol (= c global variables). store in r4.
    ldr r9, =__bss_end              // store end in r9. __bss* will be set by linker.
    mov r5, #0                      // prepare some zeros, used to overwrite the c-globals in line 19
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f                      // go to 2

1:
    stmia r4!, {r5-r8}      // write zeros to r4, then increment r4.

2:
    cmp r4, r9              // if __bss_start < __bss_end ...
    blo 1b                  // go to 1

    ldr r3, =kernel_main    // store =kernel_main in rx
    blx r3                  // move to the code in r3

halt:
    wfe
    b halt  // loop 
