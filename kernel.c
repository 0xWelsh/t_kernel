#include <stddef.h>
#include <stdint.h>

void kernel_main(void) {

	volatile char* video = (volatile char*)0xB8000;

	const char* message = "Kernel loaded successfully.";
	for (size_t i = 0; message[i] != '\0'; i++) {
	
		video[i * 2] = message[i];
		video[i * 2 + 1] = 0x07; // light grey on black
	}

	while (1) {
	
		// infinite loop so we don't return
	}

}
