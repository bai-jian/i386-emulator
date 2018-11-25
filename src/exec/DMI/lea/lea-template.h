#include "exec/template-start.h"


#include "cpu/modrm.h"

make_helper( concat(lea_m2r_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	swaddr_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);
	uint8_t  reg_i = m.reg;

	REG(reg_i) = mem_i;

	print_asm("lea" str(SUFFIX) "  %s,%%%s", ModR_M_asm, REG_NAME(reg_i));

	return 1 + len;
}


#include "exec/template-end.h"
