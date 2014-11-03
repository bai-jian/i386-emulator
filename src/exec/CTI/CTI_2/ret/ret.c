#include "exec/helper.h"


#include "memory.h"

make_helper(ret)
{
	uint32_t disp = swaddr_read(cpu.esp, 4);	cpu.esp += 4;
	cpu.eip = disp;

	return 0;
}
