#include "exec/template-start.h"


#include "cpu/reg.h"

make_helper( concat(leave_, SUFFIX) )
{
	cpu.esp = cpu.ebp;
	cpu.ebp = MEM_R(cpu.esp);
	cpu.esp += 4;

	return 1;
}


#include "exec/template-end.h"
