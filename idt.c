#include "idt.h"

extern void isr0();

struct idt_entry idt[256];
struct idt_ptr idtp;

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {

	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;

	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

static inline void lidt(struct idt_ptr* idtp) {

	__asm__ __volatile__("lidt (%0)" : : "r" (idtp));
}

void idt_init() {
	uint16_t code_selector;

	__asm__ __volatile__("mov %%cs, %0" : "=r" (code_selector));

	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = (uint32_t)&idt;

	idt_set_gate(0, (uint32_t)isr0, code_selector, 0x8E);

	lidt(&idtp);
}
