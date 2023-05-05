#! /bin/bash

set -e
set -x

nasm boot_sect.asm -f bin -o boot_sect.bin
qemu-system-x86_64 ./boot_sect.bin
