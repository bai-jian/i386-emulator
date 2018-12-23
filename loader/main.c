#include "common.h"
#include "memory.h"
#include "x86/cpu.h"

void init_page();
void init_serial();
void init_ide();
void init_i8259();
void init_segment();
void init_idt();
void init_mm();
uint32_t loader();

void video_mapping_write_test();
void video_mapping_read_test();
void video_mapping_clear();


/* Initialization phase 2 */
void init_cond( )
{
#ifdef IA32_INTR
	// Reset the GDT, since the old GDT in start.S cannot be used in the future.
	init_segment();
	// Set the IDT by setting up interrupt and exception handlers.
	// Note that system call is the only exception implemented in NEMU.
	init_idt();
#endif

	init_ide();

#ifdef IA32_PAGE
	/* Initialize the memory manager. */
	init_mm();
#endif


	// Output a welcome message.
	// Note that the output is actually performed only when the serial port is available in NEMU.
	Log("Hello, NEMU world!");


#ifdef HAS_DEVICE
	// Write some test data to the video memory
	video_mapping_write_test();
#endif


	/* Load the program. */
	uint32_t eip = loader();


#ifdef HAS_DEVICE
	// Read data in the video memory to check whether the test data is written sucessfully.
	video_mapping_read_test();
	// Clear the test data we just written in the video memory.
	video_mapping_clear();
#endif


#ifdef IA32_PAGE
	// Set the %ebp %esp for user program
	asm volatile("movl $0, %ebp");
	asm volatile("movl %0, %%esp" : : "i"(KOFFSET));
#endif


	// Stop the Kernel, indicating to run the user program
	asm volatile(".byte 0x82" : : "a"(2));

	// Here we go!!! Run the user program.
	((void(*)(void))eip)();

	HIT_GOOD_TRAP;
}

// Initialization phase 1: the assembly code in start.S will finally jump here
void init()
{
#ifdef IA32_PAGE
	/* We must set up kernel virtual memory first because our kernel thinks it 
	 * is located at 0xc0100000, which is set by the linking options in Makefile.
	 * Before setting up correct paging, no global variable can be used. */
	init_page();
	// After paging is enabled, transform %esp to virtual address.
	asm volatile("addl %0, %%esp" : : "i"(KOFFSET));
#endif

	// Jump to init_cond() to continue initialization.
	asm volatile("jmp *%0" : : "r"(init_cond));

	nemu_assert(0);    // never reach here
}
