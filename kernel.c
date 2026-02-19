#include <stddef.h>
#include <stdint.h>
#include "idt.h"

void kernel_main(void) {

	volative char* video = (volative char*)0xB8000;

	const char* message = "Kernel loaded successfully.";
	for (size_t i = 0; message[i] != '\0'; i++) {
	
		video[i * 2] = message[i];
		video[i * 2 + 1] = 0x07;
	}

	// initialize interrupt descriptor table
	idt_init();

	// trigger interrupt 0 manually
	__asm__ __volatile__("int $0");

	while (1) {
	}
}
