; Ensures that we jump straight into kernel's entry function - 
; instead of into whatever code the compiler put in first place 
; in the kernel-binary

[bits 32]
[extern main]   ; will be substituted by linker

call main
jmp $