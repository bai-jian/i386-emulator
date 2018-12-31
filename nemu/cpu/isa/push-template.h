#include "helper.h"

#include "template-start.h"

make_helper( concat(push_i_, SUFFIX) )
{
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);

	// Push imm
	if ( instr_fetch(eip, 1) == 0x6A )
	{	cpu.esp -= 4;          swaddr_write(cpu.esp, 4, imm);  }
	else
	{	cpu.esp -= DATA_BYTE;  MEM_W(cpu.esp, imm);	 }

	print_asm("push" str(SUFFIX) "  $0x%x", imm);

	return 1 + DATA_BYTE;
}

make_helper( concat(push_r_, SUFFIX) )
{ 
	uint8_t   reg_i = instr_fetch(eip, 1) & 0x07;
	DATA_TYPE reg_v = REG(reg_i);

	// Push reg
	cpu.esp -= DATA_BYTE;  MEM_W(cpu.esp, reg_v);
	
	print_asm("push" str(SUFFIX) "  %%%s", REG_NAME(reg_i));

	return 1;
}

make_helper( concat(push_m_, SUFFIX) )
{
	ModR_M m;  m.val = instr_fetch(eip + 1, 1);
	assert(m.mod != 3);

	swaddr_t  mem_i;  uint32_t len = read_ModR_M(eip+1, &mem_i);
	DATA_TYPE mem_v = MEM_R(mem_i);

	// Push mem
	cpu.esp -= DATA_BYTE;  MEM_W(cpu.esp, mem_v);

	print_asm("push" str(SUFFIX) "  %s", ModR_M_asm); 

	return 1 + len;
}

#include "template-end.h"
