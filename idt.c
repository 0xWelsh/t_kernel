#include "idt.h"

extern void *isr_stub_table[];
extern void *irq_stub_table[];

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {

	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

static inline void lidt(struct idt_ptr *p) {

	__asm__ __volatile__("lidt (%0)" : : "r"(p));
}

void idt_init(void) {
	uint32_t i;

	for (i = 0; i < 256; i++) {
		idt_set_gate((uint8_t)i, 0, 0, 0);
	}

	for (i = 0; i < 32; i++) {
		idt_set_gate((uint8_t)i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
	}

	for (i = 0; i < 16; i++) {
		idt_set_gate((uint8_t)(32 + i), (uint32_t)irq_stub_table[i], 0x08, 0x8E);
	}

	idtp.limit = (uint16_t)(sizeof(idt) - 1);
	idtp.base  = (uint32_t)&idt;
	lidt(&idtp);
}
