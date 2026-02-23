ENTRY(_start)

PHDRS
{
	text PT_LOAD FLAGS(5); /* RX */
	data PT_LOAD FLAGS(6); /* RW */
}

SECTIONS
{

	. = 1M;

	.text :
	{

		*(.multiboot)
		*(.text)
	} :text
	.rodata :
	{

		*(.rodata*)
	} :text

	.data :
	{

		*(.data)
	} :data

	.bss :
	{

		*(COMMON)
		*(.bss)
	} :data

	/DISCARD/ :
	{
		*(.note*)
		*(.eh_frame*)
	}
}
