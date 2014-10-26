#include "exec/helper.h"

#include "exec/template-start.h"

#include "cpu/modrm.h"
#include "exec/set_eflags/set_eflags.h"

make_helper( concat(test_rm1r_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		DATA_TYPE ropd = REG(m.reg);
		swaddr_t addr;
		uint32_t len = read_ModR_M(eip+1, &addr);
		DATA_TYPE lopd = MEM_R(addr);

		DATA_TYPE valu = ropd & lopd;
		cpu.CF = 0;
		cpu.OF = 0;
		concat(set_SF_, SUFFIX) (valu);
		concat(set_ZF_, SUFFIX) (valu);
		concat(set_PF_, SUFFIX) (valu);

		print_asm("test" str(SUFFIX) " %s, %%%s", ModR_M_asm, REG_NAME(m.reg));

		return 1 + len;
	}
	else
	{ 
		DATA_TYPE ropd = REG(m.reg); 
		DATA_TYPE lopd = REG(m.R_M);

		DATA_TYPE valu = ropd & lopd;
		cpu.CF = 0;  
		cpu.OF = 0;
		concat(set_SF_, SUFFIX) (valu);
		concat(set_ZF_, SUFFIX) (valu);
		concat(set_PF_, SUFFIX) (valu);

		print_asm("test" str(SUFFIX) " %%%s, %%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 1 + 1;
	}
}

#include "exec/template-end.h"
