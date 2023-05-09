
boot_sect:
	nasm boot_sect.asm -f bin -o boot_sect.bin

run: boot_sect
	qemu-system-x86_64 ./boot_sect.bin

clean:
	rm *.bin