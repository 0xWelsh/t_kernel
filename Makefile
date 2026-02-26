CC := $(shell command -v x86_64-elf-gcc 2>/dev/null || echo gcc)
AS := nasm
LD := $(CC)

CFLAGS := -ffreestanding -m32 -O2 -Wall -Wextra -fno-pie -fno-pic -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -fno-tree-vectorize -mno-mmx -mno-sse -mno-sse2 -c
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -no-pie -m32 -Wl,--build-id=none

OBJS := boot.o gdt.o gdt_asm.o isr.o kernel.o idt.o pic.o

all: myos.iso

boot.o: boot.s
	$(AS) -f elf32 boot.s -o boot.o

gdt_asm.o: gdt.s
	$(AS) -f elf32 gdt.s -o gdt_asm.o

gdt.o: gdt.c
	$(CC) $(CFLAGS) gdt.c -o gdt.o

isr.o: isr.s
	$(AS) -f elf32 isr.s -o isr.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

idt.o: idt.c
	$(CC) $(CFLAGS) idt.c -o idt.o

pic.o: pic.c
	$(CC) $(CFLAGS) pic.c -o pic.o

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.bin

myos.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o myos.iso iso

run: myos.iso
	qemu-system-x86_64 -cdrom myos.iso -boot d -machine pc

clean:
	rm -rf *.o *.bin *.iso iso
