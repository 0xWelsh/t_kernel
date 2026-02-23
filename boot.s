; Multiboot header
section .multiboot
align 4
	dd 0x1BADB002		;magic number
	dd 0x00000000		;flags
	dd -(0x1BADB002)	;checksum

section .text
global _start
extern kernel_main

_start:
	cli			; disable interrupts


	mov esp, stack_top	; set up stack

	; Multiboot passes magic in EAX and info ptr in EBX.
	; cdecl: push args right-to-left => (magic, info_ptr)
	push ebx
	push eax
	call kernel_main	; call C function
	add esp, 8

.hang:
	hlt			; halt cpu
	jmp .hang		;infinite loop

section .bss
align 16
stack_bottom:
	resb 16384		;16 kb stack
stack_top:
