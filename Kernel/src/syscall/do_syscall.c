#include "x86.h"

#include <sys/syscall.h>
#include <stdio.h>

void add_irq_handle(int, void (*)(void));
static void sys_brk(TrapFrame* tf);
static void sys_write(TrapFrame* tf);
void do_syscall(TrapFrame* tf)
{
	Log("%x %x %x\n", tf->eax, tf->irq, tf->error_code);
	Log("%x\n", SYS_brk);
	switch(tf->eax)
	{
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: add_irq_handle(tf->ebx, (void*)tf->ecx); break;

		case SYS_write:  	sys_write(tf);  break;
		case SYS_brk:  		sys_brk(tf);	break;

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

void mm_brk(uint32_t);
static void sys_brk(TrapFrame* tf)
{
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}


void serial_printc(char ch);
static void sys_write(TrapFrame* tf)
{
	uint32_t buf = tf->ecx;
	uint32_t len = tf->edx;
	uint32_t fd  = tf->ebx;

	// fd = 1: stdout;  fd = 2: stderr
	assert(fd == 1 || fd == 2);
	uint32_t i;
	for (i = 0; i < len; ++ i)
		serial_printc(*(char*)(void*)(buf + i));

	tf->eax = len;
}
