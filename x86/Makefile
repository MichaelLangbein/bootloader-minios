kernel.bin: kernel.c
	i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o
	# -Ttext 0x1000: kernel will be placed at 0x1000, so linker can make relative adresses absolute.
	i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary

boot_sect.bin: boot_sect.asm
	nasm boot_sect.asm -f bin -o boot_sect.bin

os-image: boot_sect.bin kernel.bin
	cat boot_sect.bin kernel.bin > os-image

run: os-image
	qemu-system-x86_64 -fda ./os-image

clean:
	rm *.bin
	rm *.o