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

	call kernel_main	; call C function

.hang:
	hlt			; halt cpu
	jmp .hang		;infinite loop

section .bss
align 16
stcak_bottom:
	resb 16384		;16 kb stack
stack_top:

global isr0

isr0:
	cli
	mov eax, 0xB8000
	mov byte [eax], 'I'
	mov byte [eax+1], 0x4F
	hlt
