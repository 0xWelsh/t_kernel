; isr.s (NASM, elf32)
bits 32

section .text
extern isr_handler

global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
global isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
global isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
global isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
global isr_stub_table

isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp
	call isr_handler
	add esp, 4
	
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8 		; pop int_no + err_code
	iret

%macro ISR_NOERR 2
%2:
	push dword 0
	push dword %1
	jmp isr_common_stub
%endmacro

%macro ISR_ERR 2
%2:
	push dword %1
	jmp isr_common_stub
%endmacro

; CPU exceptions 0..31
ISR_NOERR 0,  isr0
ISR_NOERR 1,  isr1
ISR_NOERR 2,  isr2
ISR_NOERR 3,  isr3
ISR_NOERR 4,  isr4
ISR_NOERR 5,  isr5
ISR_NOERR 6,  isr6
ISR_NOERR 7,  isr7
ISR_ERR   8,  isr8
ISR_NOERR 9,  isr9
ISR_ERR   10, isr10
ISR_ERR   11, isr11
ISR_ERR   12, isr12
ISR_ERR   13, isr13
ISR_ERR   14, isr14
ISR_NOERR 15, isr15
ISR_NOERR 16, isr16
ISR_ERR   17, isr17
ISR_NOERR 18, isr18
ISR_NOERR 19, isr19
ISR_NOERR 20, isr20
ISR_ERR   21, isr21
ISR_NOERR 22, isr22
ISR_NOERR 23, isr23
ISR_NOERR 24, isr24
ISR_NOERR 25, isr25
ISR_NOERR 26, isr26
ISR_NOERR 27, isr27
ISR_NOERR 28, isr28
ISR_ERR   29, isr29
ISR_ERR   30, isr30
ISR_NOERR 31, isr31

section .data
align 4
isr_stub_table:
	dd isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
	dd isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
	dd isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
	dd isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
