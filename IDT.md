the IDT (interrupt descriptor table) is just an array of entries that tell the CPU:
"if interrupt number X happens, jump to address Y"

Each entry contains:
- handler address
- segment selector
- flags

the CPU loads the IDT using the `lidt` instruction
