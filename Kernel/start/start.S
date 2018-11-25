#include "common.h"


#define GDT_ENTRY(n) ((n) << 3)

#define MAKE_NULL_SEG_DESC   \
	.word 0, 0;              \
	.byte 0, 0, 0, 0

#define MAKE_SEG_DESC(type, base, lim)                        \
	.word (((lim) >> 12) & 0xffff), ((base) & 0xffff);        \
	.byte (((base) >> 16) & 0xff), (0x90 | (type)),           \
	(0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)


	#ifdef IA32_PAGE
		#define KOFFSET 0xc0000000
		#define va_to_pa(x) (x - KOFFSET)
	#else
		#define va_to_pa(x) (x)
	#endif


.globl start
start:

	lgdt    va_to_pa(gdtdesc)

	# %CR0 |= PROTECT_ENABLE_BIT
	movl    %cr0, %eax    
	orl     $0x1, %eax
	movl    %eax, %cr0

	ljmp    $GDT_ENTRY(1), $va_to_pa(start_cond)

start_cond:

	# Set the segment registers
	movw    $GDT_ENTRY(2), %ax
	movw    %ax, %ds
	movw    %ax, %es
	movw    %ax, %ss

	# Set the stack
	movl $0,         %ebp
	movl $(1 << 27), %esp
	jmp init


	## GDT ######################################################
	.p2align 2                    # force 4 byte alignment
	gdt:
		MAKE_NULL_SEG_DESC                   # empty segment
		MAKE_SEG_DESC(0xA, 0x0, 0xffffffff)  # code
		MAKE_SEG_DESC(0x2, 0x0, 0xffffffff)  # data

	gdtdesc:                      # descriptor
		.word   (gdtdesc - gdt - 1)    # limit = sizeof(gdt) - 1
		.long   va_to_pa(gdt)          # address of GDT
	#############################################################
