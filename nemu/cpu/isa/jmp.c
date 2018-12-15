#include "helper.h"

#define  DATA_BYTE 1
#include "jmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 2
#include "jmp-template.h"
#undef   DATA_BYTE

#define  DATA_BYTE 4
#include "jmp-template.h"
#undef   DATA_BYTE


#include "cpu/reg.h"
#include "memory.h"

extern char suffix;

make_helper(jmp_i_v)
{	
	if (suffix == 'l')
		return jmp_i_l(eip);
	else
	{
		cpu.eip = cpu.eip & 0x0000FFFF;
		return jmp_i_w(eip);
	}
}

make_helper(jmp_rm)
{
	uint8_t instr_len = 1;

	ModR_M m;  m.val = instr_fetch(eip+1, 1);
	if (m.mod != 3)
	{
		swaddr_t mem_i;  instr_len += read_ModR_M(eip+1, &mem_i);

		uint32_t mem_v = swaddr_read(mem_i, 4);
		cpu.eip = mem_v - instr_len;  

		print_asm("jmp    *%s", ModR_M_asm);

		return instr_len;
	} 
	else
	{
		instr_len += 1;

		cpu.eip = reg_l(m.R_M) - instr_len;

		print_asm("jmp    *%%%s", regsl[m.R_M]);

		return instr_len;
	}
}

make_helper(jmp_seg)
{
	uint8_t instr_len = 1 + 4 + 2;

	cpu.eip = instr_fetch(eip+1, 4) - instr_len;
	cpu.CS = instr_fetch(eip+1+4, 2);
	
	print_asm("jmp    $0x%x : $0x%x", cpu.CS, cpu.eip + instr_len);
	
	return instr_len;
}
