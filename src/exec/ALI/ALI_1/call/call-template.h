#include "exec/template-start.h"


#include "cpu/reg.h"
#include "cpu/modrm.h"
#include "memory.h"

make_helper( concat(call_i_, SUFFIX) )
{
	uint8_t instr_len = 1 + DATA_BYTE;

	DATA_TYPE disp = instr_fetch(eip+1, DATA_BYTE);

	// Push the return address into stack
	cpu.esp -= DATA_BYTE;
	swaddr_write(cpu.esp, DATA_BYTE, eip + instr_len);
	// Write cpu.eip
	cpu.eip = (DATA_TYPE)cpu.eip + disp - (DATA_TYPE)instr_len;

	print_asm("call   %x", disp);

	return instr_len;
}

make_helper( concat(call_rm_, SUFFIX) )
{
	uint8_t instr_len = 1;

	ModR_M m;	m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;	 instr_len += read_ModR_M(eip+1, &mem_i);
		uint32_t addr = swaddr_read(mem_i, DATA_BYTE);

		// Push the return address into stack
		cpu.esp -= DATA_BYTE;
		swaddr_write(cpu.esp, DATA_BYTE, eip+instr_len);
		// Write eip
		cpu.eip = addr - instr_len;

		print_asm("call   *%s", ModR_M_asm);

		return instr_len;
	}
	else
	{
		instr_len += 1;
		// Push the return address into stack
		cpu.esp -= DATA_BYTE;
		swaddr_write(cpu.esp, DATA_BYTE, eip+instr_len);
		// Write eip
		cpu.eip = REG(m.R_M) - instr_len;

		print_asm("call   *%%%s", REG_NAME(m.R_M));

		return instr_len;
	}
}


#include "exec/template-end.h"
