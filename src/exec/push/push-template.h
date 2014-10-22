#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(push_r_, SUFFIX))
{
	ModR_M m;
	m.val = instr_fetch(eip+1, 1);

	cpu.esp -= DATA_BYTE;
	MEM_W( cpu.esp, REG(m.reg) );
	
	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(m.reg));

	return 1 + 1;
}

#include "exec/template-end.h"
