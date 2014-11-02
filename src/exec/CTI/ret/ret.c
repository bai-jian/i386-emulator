#include "exec/helper.h"

make_helper(ret)
{
	cpu.eip = cpu.esp;
	cpu.esp -= 4;

	return 1;
}
