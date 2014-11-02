#include "exec/helper.h"


#include "memory.h"

make_helper(ret)
{
	eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;

	return 0;
}
