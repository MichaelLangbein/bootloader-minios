compile: main.c
	i386-elf-gcc -ffreestanding -c main.c -o main.o

objdump: compile
	i386-elf-objdump -d main.o

link: compile
	i386-elf-ld -o main.bin -Ttext 0x0 --oformat binary main.o

disasm: link
	ndisasm -b 32 main.bin > main.dis
