boot_sect.bin: boot_sect.asm
	nasm boot_sect.asm -f bin -o boot_sect.bin
	
kernel_entry.o: kernel_entry.asm
	nasm kernel_entry.asm -f elf -o kernel_entry.o

kernel.o: kernel.c
	i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o

kernel.bin: kernel_entry.o kernel.o
	# -Ttext 0x1000: kernel will be placed at 0x1000, so linker can make relative adresses absolute.
	# linker respects order of files given, so kernel_entry will definitely come first
	i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

os-image: boot_sect.bin kernel.bin
	cat boot_sect.bin kernel.bin > os-image

run: os-image
	qemu-system-x86_64 -fda ./os-image

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.img
	rm os-image