#! /bin/bash

set -e
set -x

nasm boot_sect.asm -f bin -o boot_sect.bin