#include "exec/helper.h"

#include "cpu/reg.h"

make_helper(leave)
{
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp, 4);	cpu.esp -= 4;

	print_asm("leave");

	return 1;
}
