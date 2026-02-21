CC := $(shell command -v x86_64-elf-gcc 2>/dev/null || echo gcc)
AS := nasm
LD := $(CC)

CFLAGS := -ffreestanding -m32 -c
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -m32

OBJS := boot.o kernel.o idt.o

all: myos.iso

boot.o: boot.s
	$(AS) -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

idt.o: idt.c
	$(CC) $(CFLAGS) idt.c -o idt.o

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.bin

myos.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o myos.iso iso

run: myos.iso
	qemu-system-x86_64 -cdrom myos.iso -boot d

clean:
	rm -rf *.o *.bin *.iso iso
