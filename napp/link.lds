SECTIONS
{
	. = 0x100000;
	.text : { *(.main) *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss)  }
}
