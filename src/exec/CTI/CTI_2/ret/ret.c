#include "exec/helper.h"


#include "memory.h"

make_helper(ret)
{
	uint32_t disp = swaddr_read(cpu.esp, 4);	cpu.esp += 4;
	cpu.eip = disp;

	assert(cpu.eip == 0x10000b);

	print_asm(" ");

	return 0;
}
