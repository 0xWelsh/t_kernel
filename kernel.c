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

__attribute__((noreturn))
static void panic(const char* message) {
	__asm__ __volatile__("cli");
	vga_write("KERNEL PANIC:", 0x4F, 0);
	vga_write(message, 0x4F, 1);
	for (;;) {
		__asm__ __volatile__("hlt");
	}
}

void isr_handler(struct interrupt_frame* frame) {
	static const char* exception_names[32] = {
		"Divide Error", "Debug", "NMI", "Breakpoint",
		"Overflow", "BOUND", "Invalid Opcode", "Device Not Available",
		"Double Fault", "Coprocessor Segment Overrun", "Invalid TSS", "Segment Not Present",
		"Stack Fault", "General Protection Fault", "Page Fault", "Reserved",
		"x87 FP Exception", "Alignment Check", "Machine Check", "SIMD FP Exception",
		"Virtualization", "Control Protection", "Reserved", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved",
		"Hypervisor Injection", "VMM Communication", "Security", "Reserved"
	};

	if (frame->int_no < 32) {
		vga_write("EXCEPTION:", 0x4F, 2);
		vga_write(exception_names[frame->int_no], 0x4F, 3);
		panic("CPU exception");
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
