CC = x86_64-elf-gcc
AS = nasm
LD = x86_64-elf-gcc

CFLAGS = -ffreestanding -m32 -c
LDFLAGS = -T linker.ld -ffreestanding -02 -nostdlib -m32

all: iso

boot.o: boot.s
	$(AS) -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.bin

iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o myos.iso iso

run: iso
	qemu-system-x86_64 -cdrom myos.iso

clean:
	rm -rf *.o *.bin *.iso /iso/boot/kernel.bin
