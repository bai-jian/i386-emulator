#include "exec/template-start.h"


#include "cpu/modrm.h"

make_helper( concat(push_i_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip+1, DATA_BYTE);

	cpu.esp -= DATA_BYTE;
	MEM_W( cpu.esp, imm );

	print_asm("push" str(SUFFIX) " $0x%x", imm);

	return 1 + DATA_BYTE;
}

make_helper( concat(push_r_, SUFFIX) )
{ 
	uint8_t reg_code = instr_fetch(eip, 1) & 0x07;

	cpu.esp -= DATA_BYTE;
	MEM_W( cpu.esp, REG(reg_code) );
	
	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));

	return 1;
}

make_helper( concat(push_m_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.reg == 6)
	{
		swaddr_t addr;
		uint32_t len = read_ModR_M(eip+1, &addr);
		DATA_TYPE mem = MEM_R(addr);

		cpu.esp -= DATA_BYTE;
		MEM_W(cpu.esp, mem);

		print_asm("push" str(SUFFIX) " 0x%x", mem); 

		return 1 + len;
	}
	return 0;
}

#include "exec/template-end.h"
