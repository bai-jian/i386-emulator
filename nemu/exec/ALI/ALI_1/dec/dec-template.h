#include "exec/template-start.h"


#include "cpu/modrm.h"
#include "exec/set_eflags/set_eflags.h"

make_helper( concat(dec_rm_, SUFFIX) )
{
	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t addr;	int len = read_ModR_M(eip+1, &addr);

		DATA_TYPE dest = MEM_R(addr);
		DATA_TYPE valu = dest - 1;	

		MEM_W(addr, valu);
		concat(set_ZF_, SUFFIX)(valu);
		concat(set_SF_, SUFFIX)(valu);
		concat(set_PF_, SUFFIX)(valu);
		concat(set_CF_, SUFFIX)(dest, 1, 1);
		concat(set_OF_, SUFFIX)(dest, 1, 1);

		print_asm("dec" str(SUFFIX) " %s", ModR_M_asm);

		return 1 + len;
	}
	else
	{
		DATA_TYPE dest = REG(m.R_M);
		DATA_TYPE valu = dest - 1;

		REG(m.R_M) = valu;
		concat(set_ZF_, SUFFIX)(valu);
		concat(set_SF_, SUFFIX)(valu);
		concat(set_PF_, SUFFIX)(valu);
		concat(set_CF_, SUFFIX)(dest, 1, 1);
		concat(set_OF_, SUFFIX)(dest, 1, 1);
		
		print_asm("inc" str(SUFFIX) " %%%s", REG_NAME(m.R_M) );

		return 1 + 1;
	}
}   

make_helper( concat(dec_r_, SUFFIX) )
{
	int reg_i = instr_fetch(eip, 1) & 0x07;
	
	DATA_TYPE dest = REG(reg_i);
	DATA_TYPE valu = REG(reg_i) + 1;
	
	REG(reg_i) = valu;
	concat(set_ZF_, SUFFIX)(valu);
	concat(set_SF_, SUFFIX)(valu);
	concat(set_PF_, SUFFIX)(valu);
	concat(set_CF_, SUFFIX)(dest, 1, 1);
	concat(set_OF_, SUFFIX)(dest, 1, 1);

	print_asm("dec" str(SUFFIX) " %%%s", REG_NAME(reg_i) );

	return 1;
}


#include "exec/template-end.h"

	
