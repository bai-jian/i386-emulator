#include "exec/template-start.h"

#include "cpu/modrm.h"
#include "exec/set_eflags/set_eflags.h"

make_helper( concat(cmp_i82rm_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.reg == 0x7)
	{
		if (m.mod != 3)
		{
			swaddr_t addr;
			uint32_t len = read_ModR_M(eip+1, &addr);
			DATA_TYPE lopd = MEM_R(addr);
			DATA_TYPE ropd = instr_fetch(eip+1+len, DATA_BYTE);
			
			concat(set_CF_, SUFFIX) (lopd, ropd, 1);
			concat(set_SF_, SUFFIX) (lopd, ropd, 1);
			concat(set_PF_, SUFFIX) (lopd, ropd, 1);
			concat(set_ZF_, SUFFIX) (lopd, ropd, 1);
			concat(set_OF_, SUFFIX) (lopd, ropd, 1);

			print_asm("cmp" str(SUFFIX) " %.4x, %s", ropd, ModR_M_asm);

			return 1 + len + 1;
		}
		else
		{
			DATA_TYPE lopd = REG(m.R_M);
			DATA_TYPE ropd = instr_fetch(eip+1+1, DATA_BYTE);

			concat(set_CF_, SUFFIX) (lopd, ropd, 1);
			concat(set_CF_, SUFFIX) (lopd, ropd, 1);
			concat(set_CF_, SUFFIX) (lopd, ropd, 1);
			concat(set_CF_, SUFFIX) (lopd, ropd, 1);
			concat(set_CF_, SUFFIX) (lopd, ropd, 1);

			print_asm("cmp" str(SUFFIX) " %.4x, %%%s", ropd, REG_NAME(m.R_M));

			return 1 + 1 + 1;
		}
	}
	return 0;
}

#include "exec/template-end.h"
