#include "exec/template-start.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"
#include "exec/set_eflags/set_eflags.h"

make_helper( concat(neg_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	if (m.mod != 3)
 	{
		swaddr_t mem_a;  uint8_t len = read_ModR_M(eip + 1, &mem_a);

		DATA_TYPE opd = MEM_R(mem_a);
		DATA_TYPE val = -opd;
		MEM_W(mem_a, val);
		cpu.CF = !(opd == 0);
		concat(set_ZF_, SUFFIX) (val);
		concat(set_SF_, SUFFIX) (val);
		concat(set_PF_, SUFFIX) (val);
		concat(set_OF_, SUFFIX) (0, opd, 1);

		print_asm("neg"str(SUFFIX)"   %s", ModR_M_asm);
		return 1 + len;
	}
	else
	{
		DATA_TYPE opd = REG(m.R_M);
		DATA_TYPE val = -opd;
		REG(m.R_M) = val;
		cpu.CF = !(opd == 0);
		concat(set_ZF_, SUFFIX) (val);
		concat(set_SF_, SUFFIX) (val);
		concat(set_PF_, SUFFIX) (val);
		concat(set_OF_, SUFFIX) (0, opd, 1);

		print_asm("neg"str(SUFFIX)"   %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
}


#include "exec/template-end.h"
