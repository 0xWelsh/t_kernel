The IDT (Interrupt Descriptor Table) is an array of entries that tells the CPU:
"if interrupt number X happens, jump to address Y"

Each entry contains:
- handler address
- segment selector
- flags

The CPU loads the IDT using the `lidt` instruction.
