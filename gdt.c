#include "gdt.h"

extern void gdt_flush(uint32_t gdt_ptr_addr);

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

static void gdt_set_gate(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[index].base_low = (uint16_t)(base & 0xFFFF);
	gdt[index].base_middle = (uint8_t)((base >> 16) & 0xFF);
	gdt[index].base_high = (uint8_t)((base >> 24) & 0xFF);

	gdt[index].limit_low = (uint16_t)(limit & 0xFFFF);
	gdt[index].granularity = (uint8_t)((limit >> 16) & 0x0F);
	gdt[index].granularity |= (uint8_t)(gran & 0xF0);
	gdt[index].access = access;
}

void gdt_init(void) {
	gp.limit = (uint16_t)(sizeof(struct gdt_entry) * 3 - 1);
	gp.base = (uint32_t)&gdt;

	/* null descriptor */
	gdt_set_gate(0, 0, 0, 0, 0);
	/* kernel code: base 0, limit 4 GiB, ring 0, 32-bit, 4K granularity */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	/* kernel data: base 0, limit 4 GiB, ring 0, 32-bit, 4K granularity */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	gdt_flush((uint32_t)&gp);
}
