#include "exec/template-start.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"

make_helper( concat(lgdt_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		uint32_t mem_i;  uint8_t len = read_ModR_M(eip+1, &mem_i);

		DATA_TYPE mem_v = MEM_R(mem_i);
		cpu.GDTR = mem_v;

		return 1 + len;
	}
	assert(0);
}


#include "exec/template-end.h"
