CC := $(shell command -v x86_64-elf-gcc 2>/dev/null || echo gcc)
AS = nasm
LD := $(CC)

CFLAGS = -ffreestanding -m32 -c
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -m32

all: iso

boot.o: boot.s
	$(AS) -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

kernel.bin: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.bin

iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	@if [ -f grub.cfg ]; then cp grub.cfg iso/boot/grub/; fi
	grub-mkrescue -o myos.iso iso

run: iso
	qemu-system-x86_64 -cdrom myos.iso -display gtk

clean:
	rm -rf *.o *.bin *.iso iso/boot/kernel.bin
