#include <stddef.h>
#include <stdint.h>
#include "idt.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

static void vga_write(const char* message, uint8_t color, size_t row) {
	volatile char* video = (volatile char*)0xB8000;
	size_t base = row * 80;
	for (size_t i = 0; message[i] != '\0'; i++) {
		video[(base + i) * 2] = message[i];
		video[(base + i) * 2 + 1] = color;
	}
}

void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info_addr) {

	(void)multiboot_info_addr;

	if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		vga_write("Boot error: invalid multiboot magic.", 0x4F, 0);
		for (;;) {
			__asm__ __volatile__("hlt");
		}
	}

	vga_write("Kernel loaded successfully.", 0x07, 0);
	vga_write("Multiboot handoff verified.", 0x0A, 1);

	// initialize interrupt descriptor table
	idt_init();

	// trigger interrupt 0 manually
	__asm__ __volatile__("int $0");

	while (1) {
		__asm__ __volatile__("hlt");
	}
}
