# Special variables:
# $^ is substituted with all of the target's dependency-files
# $< is the first dependency 
# $@ is the target-file


boot_sect.bin: ./boot/boot_sect.asm
	nasm $< -f bin -o $@
	
kernel_entry.o: ./kernel/kernel_entry.asm
	nasm $< -f elf -o $@

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
OBJ = ${C_SOURCES:.c=.o}

%.o: %.c
	i386-elf-gcc -ffreestanding -c $< -o $@

kernel.bin: kernel_entry.o ${OBJ}
	# -Ttext 0x1000: kernel will be placed at 0x1000, so linker can make relative adresses absolute.
	# linker respects order of files given, so kernel_entry will definitely come first
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

os-image: boot_sect.bin kernel.bin
	cat boot_sect.bin kernel.bin > $@

run: os-image
	qemu-system-x86_64 -fda ./$<

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.img
	rm os-image