#include <stddef.h>
#include <stdint.h>
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "pic.h"

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

	if (frame->int_no >= 32 && frame->int_no < 48) {
		uint32_t irq = frame->int_no - 32;

		if (irq == 0) {
			static uint32_t ticks;
			ticks++;
			if ((ticks % 100) == 0) {
				vga_write("Timer IRQ alive.", 0x0B, 4);
			}
		}

		if (irq == 1) {
			(void)inb(0x60);
			vga_write("Keyboard IRQ received.", 0x0E, 5);
		}

		pic_send_eoi((uint8_t)irq);
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

	// initialize global descriptor table
	gdt_init();

	// remap PIC from 0x08-0x0F/0x70-0x77 to 0x20-0x2F
	pic_remap(0x20, 0x28);

	// initialize interrupt descriptor table
	idt_init();

	vga_write("Interrupts initialized.", 0x0A, 2);
	__asm__ __volatile__("sti");
	vga_write("Interrupts enabled.", 0x0A, 3);

	while (1) {
		__asm__ __volatile__("hlt");
	}
}
